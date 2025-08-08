// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BSCharacterEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BrainComponent.h"

#include "AI/Controller/BSEnemyAIController.h"
#include "Components/BSStateComponent.h"
#include "Components/BSAttributeComponent.h"
#include "Components/BSCombatComponent.h"
#include "Components/BSRotationComponent.h"
#include "Equipments/BSWeapon.h"
#include "Items/BSPickupItem.h"
#include "UI/BSStatBarWidget.h"
#include "BSDefine.h"
#include "BSGameplayTag.h"

ABSCharacterEnemy::ABSCharacterEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	StateComp = CreateDefaultSubobject<UBSStateComponent>(TEXT("State Component"));

	AttributeComp = CreateDefaultSubobject<UBSAttributeComponent>(TEXT("Attribute Component"));
	AttributeComp->OnChangedAttribute.AddUObject(this, &ThisClass::OnChangedAttribute);
	AttributeComp->OnDeath.AddUObject(this, &ThisClass::OnDeath);
	AttributeComp->OnMaxPosture.AddUObject(this, &ThisClass::OnPosture);

	CombatComp = CreateDefaultSubobject<UBSCombatComponent>(TEXT("Combat Component"));

	RotationComp = CreateDefaultSubobject<UBSRotationComponent>(TEXT("Rotation Component"));
	RotationComp->bOwnerIsAI = true;

	HealthBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	HealthBarWidgetComp->SetupAttachment(GetRootComponent());
	HealthBarWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HealthBarWidgetComp->SetDrawSize(FVector2D(100.f, 10.f));
	HealthBarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComp->SetVisibility(false);

	BackAttackWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("BackAttack Widget Component"));
	BackAttackWidgetComp->SetupAttachment(GetRootComponent());
	BackAttackWidgetComp->SetDrawSize(FVector2D(30.f, 30.f));
	BackAttackWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	BackAttackWidgetComp->SetVisibility(false);

	PostureWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Posture Widget Component"));
	PostureWidgetComp->SetupAttachment(GetRootComponent());
	PostureWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	PostureWidgetComp->SetDrawSize(FVector2D(100.f, 5.f));
	PostureWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	PostureWidgetComp->SetVisibility(false);

	PostureAttackWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("PostureAttack Widget Component"));
	PostureAttackWidgetComp->SetupAttachment(GetRootComponent());
	PostureAttackWidgetComp->SetDrawSize(FVector2D(70.f, 70.f));
	PostureAttackWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	PostureAttackWidgetComp->SetVisibility(false);

	// Targeting 구체 생성및 Collision 설정.
	TargetingSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Targeting Sphere Component"));
	TargetingSphereComp->SetupAttachment(GetRootComponent());
	TargetingSphereComp->SetCollisionObjectType(COLLISION_OBJECT_TARGETING);
	TargetingSphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphereComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	LockOnWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Lock On Widget Component"));
	LockOnWidgetComp->SetupAttachment(GetRootComponent());
	LockOnWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	LockOnWidgetComp->SetDrawSize(FVector2D(30.f, 30.f));
	LockOnWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidgetComp->SetVisibility(false);

	TeamId = FGenericTeamId(1);
}

void ABSCharacterEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultWeaponClass)
	{
		FActorSpawnParameters Param;
		Param.Owner = this;

		ABSWeapon* Weapon = GetWorld()->SpawnActor<ABSWeapon>(DefaultWeaponClass, GetActorTransform(), Param);
		Weapon->EquipItem();
	}

	SetupAttribute();
}

void ABSCharacterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABSCharacterEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	check(AttributeComp);

	// 대치 중인가
	const bool bFacing = UKismetMathLibrary::InRange_FloatFloat(GetDotProductTo(EventInstigator->GetPawn()), -0.1f, 1.f);

	if (IsBlockingState() && bFacing)
	{
		ImpactEffect(GetActorLocation());
		return 0.f;
	}

	if (bTakeDamageUsage)
	{
		AttributeComp->TakeDamageAmount(ActualDamage);

		if (!IsEnabledPostureAttack() && !IsBlockingState())
		{
			AttributeComp->TakePostureAmount(ActualDamage);
			AttributeComp->TogglePostureRegen(false);
			AttributeComp->TogglePostureRegen(true, 2.f);
		}
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		FVector ShotDirection = PointDamageEvent->ShotDirection;
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;
		FVector HitLocation = PointDamageEvent->HitInfo.Location;

		// AI가 데미지를 인식할 수 있도록 알려줌.
		UAISense_Damage::ReportDamageEvent(GetWorld(), this, EventInstigator->GetPawn(), ActualDamage, HitLocation, HitLocation);

		ImpactEffect(ImpactPoint);

		HitReaction(EventInstigator->GetPawn(), EDamageType::HitBack);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		const FRadialDamageEvent* RadialDamageEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);

		const FVector HitLocation = RadialDamageEvent->Origin;

		// AI가 데미지를 인식할 수 있도록 알려줌.
		UAISense_Damage::ReportDamageEvent(GetWorld(), this, EventInstigator->GetPawn(), ActualDamage, HitLocation, HitLocation);

		ImpactEffect(HitLocation);

		HitReaction(EventInstigator->GetPawn(), EDamageType::KnockBack);
	}

	return ActualDamage;
}

void ABSCharacterEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(ParriedDelayTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StunnedDelayTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void ABSCharacterEnemy::ActivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType)
{
	if (CombatComp)
	{
		CombatComp->GetMainWeapon()->ActivateWeaponCollision(WeaponCollisionType);
	}
}

void ABSCharacterEnemy::DeactivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType)
{
	if (CombatComp)
	{
		CombatComp->GetMainWeapon()->DeactivateWeaponCollision(WeaponCollisionType);
	}
}

void ABSCharacterEnemy::PerformAttack(const FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate)
{
	check(StateComp);
	check(CombatComp);
	check(AttributeComp);

	if (StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_MaxPosture)) return;

	if (const ABSWeapon* Weapon = CombatComp->GetMainWeapon())
	{
		StateComp->SetState(BSGameplayTag::Character_State_Attacking);
		CombatComp->SetLastAttackType(AttackTypeTag);
		AttributeComp->ToggleStaminaRegen(false);

		if (UAnimMontage* Montage = Weapon->GetRandomMontageForTag(AttackTypeTag))
		{
			if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
			{
				Anim->Montage_Play(Montage);
				Anim->Montage_SetEndDelegate(MontageEndedDelegate, Montage);
			}
		}

		const float StaminaCost = Weapon->GetStaminaCost(AttackTypeTag);
		AttributeComp->DecreaseStamina(StaminaCost);
		AttributeComp->ToggleStaminaRegen(true);
	}
}

void ABSCharacterEnemy::Parried()
{
	check(StateComp);
	check(CombatComp);

	if (StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_MaxPosture)) return;

	StateComp->SetState(BSGameplayTag::Character_State_Parried);
	StopAnimMontage();

	if (const ABSWeapon* MainWeapon = CombatComp->GetMainWeapon())
	{
		UAnimMontage* ParriedAnimMontage = MainWeapon->GetMontageForTag(BSGameplayTag::Character_Action_ParriedHit);
		const float Delay = PlayAnimMontage(ParriedAnimMontage) + 1.f;

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]()
			{
				FGameplayTagContainer CheckTags;
				CheckTags.AddTag(BSGameplayTag::Character_State_Death);
				CheckTags.AddTag(BSGameplayTag::Character_State_MaxPosture);
				if (StateComp->IsCurrentStateEqualToAny(CheckTags) == false)
				{
					StateComp->ClearState();
				}
			});
		GetWorld()->GetTimerManager().SetTimer(ParriedDelayTimerHandle, TimerDelegate, Delay, false);
	}
}

void ABSCharacterEnemy::EnemyBlocking()
{
}

void ABSCharacterEnemy::EnemyDodge()
{
}

void ABSCharacterEnemy::ToggleBackAttackWidgetVisibility(const bool bShouldBackAttack) const
{
	if (BackAttackWidgetComp)
	{
		BackAttackWidgetComp->SetVisibility(bShouldBackAttack);
	}
}

void ABSCharacterEnemy::BackAttacked(UAnimMontage* BackAttackReactionMontage)
{
	check(StateComp);
	check(AttributeComp);

	StopAnimMontage();

	StateComp->SetState(BSGameplayTag::Character_State_BackAttacked);

	PlayAnimMontage(BackAttackReactionMontage);

	AttributeComp->TakeDamageAmount(999.f);
}

bool ABSCharacterEnemy::IsEnabledPostureAttack() const
{
	check(StateComp);

	return StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_MaxPosture);
}

void ABSCharacterEnemy::TogglePostureAttackWidgetVisibility(const bool bShouldPostureAttack) const
{
	if (PostureAttackWidgetComp)
	{
		PostureAttackWidgetComp->SetVisibility(bShouldPostureAttack);
	}
}

void ABSCharacterEnemy::PostureAttacked(UAnimMontage* PostureAttackReactionMontage)
{
	check(AttributeComp);
	check(StateComp);

	StateComp->SetState(BSGameplayTag::Character_State_MaxPostureAttacked);

	StopAnimMontage();

	PlayAnimMontage(PostureAttackReactionMontage);

	AttributeComp->TakeDamageAmount(150.f);
	AttributeComp->TogglePostureRegen(true);
}

void ABSCharacterEnemy::OnTargeted(bool bTargeted)
{
	if (LockOnWidgetComp)
	{
		LockOnWidgetComp->SetVisibility(bTargeted);
	}
}

bool ABSCharacterEnemy::CanBeTargeted()
{
	check(StateComp);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Death);
	return StateComp->IsCurrentStateEqualToAny(CheckTags) == false;
}

void ABSCharacterEnemy::SeesTarget(AActor* InTargetActor)
{
	// AIController 에서 호출하기 위한 가상함수
	if (bUnstoppable)
	{
		GetMesh()->SetOverlayMaterial(OutlineMaterial);
	}
}

void ABSCharacterEnemy::ToggleHealthBarVisibility(bool bVisibility) const
{
	if (HealthBarWidgetComp)
	{
		HealthBarWidgetComp->SetVisibility(bVisibility);
	}

	if (PostureWidgetComp)
	{
		PostureWidgetComp->SetVisibility(bVisibility);
	}
}

void ABSCharacterEnemy::OnDeath()
{
	check(StateComp);
	check(CombatComp);

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBrainComponent()->StopLogic(TEXT("Death"));
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_BackAttacked);
	CheckTags.AddTag(BSGameplayTag::Character_State_MaxPostureAttacked);

	if (!StateComp->IsCurrentStateEqualToAny(CheckTags))
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
		GetMesh()->SetSimulatePhysics(true);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 메쉬의 콜리전을 바꿔주지 않으면 1대가 남아있던데 왜 그런지 모르겠음;
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}

	if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
	{
		ABSPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<ABSPickupItem>(ABSPickupItem::StaticClass(), GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		PickupItem->SetEquipmentClass(Weapon->GetClass());
		PickupItem->FinishSpawning(GetMesh()->GetSocketTransform(TEXT("hand_rSocket")));

		Weapon->Destroy();
	}

	SetDeathState();
}

void ABSCharacterEnemy::SetDeathState()
{
	if (ABSEnemyAIController* AIController = Cast<ABSEnemyAIController>(GetController()))
	{
		AIController->StopUpdateTarget();
	}

	ToggleHealthBarVisibility(false);
	ToggleBackAttackWidgetVisibility(false);
	TogglePostureAttackWidgetVisibility(false);

	if (bUnstoppable)
	{
		GetMesh()->SetOverlayMaterial(nullptr);
	}

	FTimerHandle DeathTimer;
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, [this]()
		{
			Destroy();
		}, 10.f, false);
}

void ABSCharacterEnemy::ImpactEffect(const FVector& Location)
{
	if (IsBlockingState())
	{
		BlockImpactEffect(Location);
	}
	else
	{
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
		}

		if (ImpactParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Location);
		}
	}
}

void ABSCharacterEnemy::BlockImpactEffect(const FVector& Location)
{
	if (BlockingImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlockingImpactSound, Location);
	}

	if (BlockingImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BlockingImpactParticle, Location);
	}
}

void ABSCharacterEnemy::HitReaction(const AActor* Attacker, const EDamageType& DamageType)
{
	check(CombatComp);
	check(StateComp);

	if (StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_MaxPosture)) return;

	float StunnedTime = 0.f;
	if (StunnedRate >= FMath::RandRange(1, 100))
	{
		StateComp->SetState(BSGameplayTag::Character_State_Stunned);
		StunnedTime = FMath::RandRange(0.5f, 1.5f);
	}

	if (!bUnstoppable)
	{
		if (ABSWeapon* MainWeapon = CombatComp->GetMainWeapon())
		{
			if (UAnimMontage* HitReactAnimMontage = MainWeapon->GetHitReactMontage(Attacker))
			{
				const float DelaySeconds = PlayAnimMontage(HitReactAnimMontage) + StunnedTime;

				FTimerDelegate TimerDelegate;
				TimerDelegate.BindLambda([this]()
					{
						FGameplayTagContainer CheckTags;
						CheckTags.AddTag(BSGameplayTag::Character_State_Death);
						CheckTags.AddTag(BSGameplayTag::Character_State_MaxPosture);
						if (StateComp->IsCurrentStateEqualToAny(CheckTags) == false)
						{
							StateComp->ClearState();
						}
					});
				GetWorld()->GetTimerManager().SetTimer(StunnedDelayTimerHandle, TimerDelegate, DelaySeconds, false);
			}
		}
	}
}

void ABSCharacterEnemy::OnChangedAttribute(const EAttributeType& AttributeType, float InRatio)
{
	switch (AttributeType)
	{
	case EAttributeType::Health:
		if (HealthBarWidgetComp)
		{
			if (UBSStatBarWidget* StatBarWidget = Cast<UBSStatBarWidget>(HealthBarWidgetComp->GetWidget()))
			{
				StatBarWidget->SetStatBarRatio(InRatio);
			}
		}
		break;
	case EAttributeType::Posture:
		if (PostureWidgetComp)
		{
			if (UBSStatBarWidget* StatBarWidget = Cast<UBSStatBarWidget>(PostureWidgetComp->GetWidget()))
			{
				StatBarWidget->SetStatBarRatio(InRatio);
			}
		}
		break;
	}
}

void ABSCharacterEnemy::SetupAttribute()
{
	if (AttributeComp)
	{
		AttributeComp->BroadcastAttributeChanged(EAttributeType::Health);
		AttributeComp->BroadcastAttributeChanged(EAttributeType::Posture);
	}
}

void ABSCharacterEnemy::OnPosture()
{
	if (bUnstoppable) return;

	check(CombatComp);
	check(StateComp);

	StateComp->SetState(BSGameplayTag::Character_State_MaxPosture);

	if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
	{
		if (UAnimMontage* Montage = Weapon->GetMontageForTag(BSGameplayTag::Character_State_MaxPosture))
		{
			PlayAnimMontage(Montage);

			UE_LOG(LogTemp, Display, TEXT("Max Posture"));
			// 급소 공격을 할 수 있는 어떤 표시..
		}
	}
}

bool ABSCharacterEnemy::IsBlockingState() const
{
	if (StateComp)
	{
		return StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_Blocking) && bEnabledBlocking;
	}
	return false;
}
