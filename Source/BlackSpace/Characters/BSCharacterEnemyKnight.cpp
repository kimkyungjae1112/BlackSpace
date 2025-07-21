// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BSCharacterEnemyKnight.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"

#include "UI/BSBossHealthBarWidget.h"
#include "Interface/BSUpdateAnyTypeInterface.h"
#include "Components/BSStateComponent.h"
#include "Components/BSAttributeComponent.h"
#include "Components/BSCombatComponent.h"
#include "AI/Controller/BSEnemyAIController.h"
#include "Equipments/BSWeapon.h"

ABSCharacterEnemyKnight::ABSCharacterEnemyKnight()
{
}

void ABSCharacterEnemyKnight::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

float ABSCharacterEnemyKnight::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{
		if (Anim->Montage_IsPlaying(SecondPhaseMontage))
		{
			ActualDamage /= 5.f;
			AttributeComp->TakePostureAmount(ActualDamage);
			return ActualDamage;
		}
	}

	check(StateComp);
	check(AttributeComp);
	check(CombatComp);

	// 대치 중인가
	const bool bFacing = UKismetMathLibrary::InRange_FloatFloat(GetDotProductTo(EventInstigator->GetPawn()), -0.1f, 1.f);

	AttributeComp->TakeDamageAmount(ActualDamage);

	if (AttributeComp->GetHealthRatio() <= 0.5f && !bIsActiveSecondPhase)
	{
		ChangeWeapon();

		bIsActiveSecondPhase = true;
		StateComp->SetState(BSGameplayTag::Character_State_Stunned);
		PlayAnimMontage(SecondPhaseMontage);
	}

	if (!IsEnabledPostureAttack() && !IsBlockingState())
	{
		AttributeComp->TakePostureAmount(ActualDamage);
		AttributeComp->TogglePostureRegen(false);
		AttributeComp->TogglePostureRegen(true, 2.f);
	}

	if (IsBlockingState() && bFacing)
	{
		if (ParryingAttackRate >= FMath::RandRange(1, 100))
		{
			if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
			{
				if (UAnimMontage* Montage = Weapon->GetMontageForTag(BSGameplayTag::Character_Attack_Special))
				{
					PlayAnimMontage(Montage);
				}
			}
		}
	}

	return 0.0f;
}

void ABSCharacterEnemyKnight::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(BlockingDelayTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void ABSCharacterEnemyKnight::EnemyBlocking()
{
	if (ABSEnemyAIController* AIController = Cast<ABSEnemyAIController>(GetController()))
	{
		if (CanBlocking() && BlockingRate >= FMath::RandRange(1, 100) && AIController->ToTargetDist() <= 300.f)
		{
			StateComp->SetState(BSGameplayTag::Character_State_Blocking);
			
			if (IBSUpdateAnyTypeInterface* AnimInterface = Cast<IBSUpdateAnyTypeInterface>(GetMesh()->GetAnimInstance()))
			{
				AnimInterface->UpdateBlcokingState(true);
				bEnabledBlocking = true;
			}

			FLatentActionInfo LatentAction;
			LatentAction.CallbackTarget = this;
			LatentAction.ExecutionFunction = "BlockingEnableAction";
			LatentAction.Linkage = 0;
			LatentAction.UUID = 0;

			UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 2.f, LatentAction);
		}
	}
}

void ABSCharacterEnemyKnight::EnemyDodge()
{

}

void ABSCharacterEnemyKnight::PostureAttacked(UAnimMontage* PostureAttackReactionMontage)
{
	Super::PostureAttacked(PostureAttackReactionMontage);

	PlayAnimMontage(PostureAttackReactionMontage);
}

void ABSCharacterEnemyKnight::LoadBodyMeshMaterial() const
{
	UMaterialInterface* ArmMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/_Assets/DF_DRAGON_KNIGHT/MATERIALS/INSTANCES/Fire/MI_DK_Arms_fire.MI_DK_Arms_fire'"));
	UMaterialInterface* BodyMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/_Assets/DF_DRAGON_KNIGHT/MATERIALS/INSTANCES/Fire/MI_DK_Body_Fire.MI_DK_Body_Fire'"));
	UMaterialInterface* ClothMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/_Assets/DF_DRAGON_KNIGHT/MATERIALS/INSTANCES/Fire/MI_DK_Cloth_Fire.MI_DK_Cloth_Fire'"));
	UMaterialInterface* SwordMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/_Assets/DF_DRAGON_KNIGHT/MATERIALS/INSTANCES/Fire/MI_DK_Sword_Fire.MI_DK_Sword_Fire'"));

	if (GetMesh())
	{
		if (ArmMat)
		{
			GetMesh()->SetMaterial(0, ArmMat);
		}

		if (BodyMat)
		{
			GetMesh()->SetMaterial(1, BodyMat);
		}

		if (ClothMat)
		{
			GetMesh()->SetMaterial(3, ClothMat);
			GetMesh()->SetMaterial(4, ClothMat);
		}
	}

	if (CombatComp)
	{
		if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
		{
			Weapon->MeshComp->SetMaterial(0, SwordMat);
		}
	}
}

void ABSCharacterEnemyKnight::SeesTarget(AActor* InTargetActor)
{
	if (IsValid(InTargetActor))
	{
		if (BossHealthBarWidget)
		{
			BossHealthBarWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	if (KnightMusicAsset)
	{
		if (!bStartedBossMusic)
		{
			bStartedBossMusic = true;
			KnightMusicComp = UGameplayStatics::SpawnSound2D(this, KnightMusicAsset);
			KnightMusicComp->FadeIn(1.f);
		}
	}
}

void ABSCharacterEnemyKnight::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidgetComp)
	{
		HealthBarWidgetComp->DestroyComponent();
	}

	if (BackAttackWidgetComp)
	{
		BackAttackWidgetComp->DestroyComponent();
	}

	if (PostureWidgetComp)
	{
		PostureWidgetComp->DestroyComponent();
	}

	if (BossHealthBarWidgetClass)
	{
		BossHealthBarWidget = CreateWidget<UBSBossHealthBarWidget>(GetWorld(), BossHealthBarWidgetClass);
		if (BossHealthBarWidget)
		{
			BossHealthBarWidget->Init(AttributeComp, BossName);
			BossHealthBarWidget->AddToViewport();
			BossHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	OnAttackStart.AddUObject(this, &ThisClass::EnemyBlocking);
}

void ABSCharacterEnemyKnight::OnDeath()
{
	Super::OnDeath();

	if (BossHealthBarWidget)
	{
		BossHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (IsValid(KnightMusicComp))
	{
		KnightMusicComp->FadeOut(2.f, 0);
	}
}

void ABSCharacterEnemyKnight::HitReaction(const AActor* Attacker, const EDamageType& DamageType)
{
}

bool ABSCharacterEnemyKnight::CanBlocking() const
{
	check(StateComp);
	check(AttributeComp);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Attacking);
	CheckTags.AddTag(BSGameplayTag::Character_State_Hit);
	CheckTags.AddTag(BSGameplayTag::Character_State_MaxPosture);
	CheckTags.AddTag(BSGameplayTag::Character_State_Parried);
	CheckTags.AddTag(BSGameplayTag::Character_State_Stunned);

	return StateComp->IsCurrentStateEqualToAny(CheckTags) == false;
}

void ABSCharacterEnemyKnight::BlockingEnableAction()
{
	check(StateComp);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Death);
	CheckTags.AddTag(BSGameplayTag::Character_State_MaxPosture);
	CheckTags.AddTag(BSGameplayTag::Character_State_Stunned);
	if (StateComp->IsCurrentStateEqualToAny(CheckTags) == false)
	{
		StateComp->ClearState();

		if (IBSUpdateAnyTypeInterface* AnimInterface = Cast<IBSUpdateAnyTypeInterface>(GetMesh()->GetAnimInstance()))
		{
			AnimInterface->UpdateBlcokingState(false);
			bEnabledBlocking = false;
		}
	}
}

void ABSCharacterEnemyKnight::ChangeWeapon()
{
	CombatComp->SetUnequipMainWeapon();

	if (GreateSwordWeaponClass)
	{
		FActorSpawnParameters Param;
		Param.Owner = this;

		ABSWeapon* GreateSword = GetWorld()->SpawnActor<ABSWeapon>(GreateSwordWeaponClass, GetActorTransform(), Param);
		if (GreateSword)
		{
			GreateSword->EquipItem();
		}
	}
}
