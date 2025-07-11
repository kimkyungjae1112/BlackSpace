// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BSCharacterEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
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

	CombatComp = CreateDefaultSubobject<UBSCombatComponent>(TEXT("Combat Component"));

	RotationComp = CreateDefaultSubobject<UBSRotationComponent>(TEXT("Rotation Component"));
	RotationComp->bOwnerIsAI = true;

	HealthBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	HealthBarWidgetComp->SetupAttachment(GetRootComponent());
	HealthBarWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HealthBarWidgetComp->SetDrawSize(FVector2D(100.f, 5.f));
	HealthBarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComp->SetVisibility(false);

	BackAttackWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("BackAttack Widget Component"));
	BackAttackWidgetComp->SetupAttachment(GetRootComponent());
	BackAttackWidgetComp->SetDrawSize(FVector2D(30.f, 30.f));
	BackAttackWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	BackAttackWidgetComp->SetVisibility(false);
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

	if (AttributeComp)
	{
		AttributeComp->TakeDamageAmount(ActualDamage);
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

		HitReaction(EventInstigator->GetPawn());
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

	StopAnimMontage();
	StateComp->SetState(BSGameplayTag::Character_State_Parried);

	if (const ABSWeapon* MainWeapon = CombatComp->GetMainWeapon())
	{
		UAnimMontage* ParriedAnimMontage = MainWeapon->GetMontageForTag(BSGameplayTag::Character_Action_ParriedHit);
		const float Delay = PlayAnimMontage(ParriedAnimMontage) + 1.f;

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]()
			{
				if (StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_Death) == false)
				{
					StateComp->ClearState();
				}
			});
		GetWorld()->GetTimerManager().SetTimer(ParriedDelayTimerHandle, TimerDelegate, Delay, false);
	}
}

void ABSCharacterEnemy::ToggleBackAttackWidgetVisibility(const bool bShouldBackAttack)
{
	BackAttackWidgetComp->SetVisibility(bShouldBackAttack);
}

void ABSCharacterEnemy::BackAttacked(UAnimMontage* BackAttackReactionMontage)
{
	check(StateComp);
	check(AttributeComp);

	StopAnimMontage();

	StateComp->SetState(BSGameplayTag::Character_State_BackAttacked);

	PlayAnimMontage(BackAttackReactionMontage);

	AttributeComp->TakeDamageAmount(9999.f);
}

void ABSCharacterEnemy::ToggleHealthBarVisibility(bool bVisibility) const
{
	HealthBarWidgetComp->SetVisibility(bVisibility);
}

void ABSCharacterEnemy::OnDeath()
{
	check(StateComp);
	check(CombatComp);

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBrainComponent()->StopLogic(TEXT("Death"));
	}

	if (!StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_BackAttacked))
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

	FTimerHandle DeathTimer;
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, [this]()
		{
			Destroy();
		}, 10.f, false);
}

void ABSCharacterEnemy::ImpactEffect(const FVector& Location)
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

void ABSCharacterEnemy::HitReaction(const AActor* Attacker)
{
	check(CombatComp);
	check(StateComp);

	float StunnedTime = 0.f;
	if (StunnedRate >= FMath::RandRange(1, 100))
	{
		StateComp->SetState(BSGameplayTag::Character_State_Stunned);
		StunnedTime = FMath::RandRange(0.5f, 1.5f);
	}

	if (ABSWeapon* MainWeapon = CombatComp->GetMainWeapon())
	{
		if (UAnimMontage* HitReactAnimMontage = MainWeapon->GetHitReactMontage(Attacker))
		{
			const float DelaySeconds = PlayAnimMontage(HitReactAnimMontage) + StunnedTime;

			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([this]()
				{
					if (StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_Stunned))
					{
						StateComp->ClearState();
					}
				});
			GetWorld()->GetTimerManager().SetTimer(StunnedDelayTimerHandle, TimerDelegate, DelaySeconds, false);
		}
	}
}

void ABSCharacterEnemy::OnChangedAttribute(const EAttributeType& AttributeType, float InRatio)
{
	if (AttributeType == EAttributeType::Health)
	{
		if (UBSStatBarWidget* StatBarWidget = Cast<UBSStatBarWidget>(HealthBarWidgetComp->GetWidget()))
		{
			StatBarWidget->SetStatBarRatio(InRatio);
		}
	}
}

void ABSCharacterEnemy::SetupAttribute()
{
	if (AttributeComp)
	{
		AttributeComp->BroadcastAttributeChanged(EAttributeType::Health);
	}
}


