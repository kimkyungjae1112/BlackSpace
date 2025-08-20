// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BSCharacterEnemyMaldrith.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "UI/BSBossHealthBarWidget.h"
#include "GameModes/BSAudioManagerSubsystem.h"
#include "GameModes/BSGameMode.h"
#include "Equipments/BSWeapon.h"
#include "Components/BSStateComponent.h"

ABSCharacterEnemyMaldrith::ABSCharacterEnemyMaldrith()
{
}

void ABSCharacterEnemyMaldrith::PostureAttacked(UAnimMontage* PostureAttackReactionMontage)
{
	Super::PostureAttacked(PostureAttackReactionMontage);

	PlayAnimMontage(PostureAttackReactionMontage);
}

void ABSCharacterEnemyMaldrith::EquipWeapon()
{
	if (DefaultWeaponClass && bSeparatedWeaponEquip)
	{
		FActorSpawnParameters Param;
		Param.Owner = this;

		ABSWeapon* Weapon = GetWorld()->SpawnActor<ABSWeapon>(DefaultWeaponClass, GetActorTransform(), Param);
		Weapon->EquipItem();
	}
}

void ABSCharacterEnemyMaldrith::PlayEquipMontage()
{
	check(StateComp);

	PlayAnimMontage(EquipMontage);
}

AController* ABSCharacterEnemyMaldrith::GetBossController() const
{
	return GetController();
}

void ABSCharacterEnemyMaldrith::SeesTarget(AActor* InTargetActor)
{
	if (IsValid(InTargetActor))
	{
		if (BossHealthBarWidget)
		{
			BossHealthBarWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		if (BossHealthBarWidget)
		{
			BossHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (ABSGameMode* GameMode = Cast<ABSGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (!GameMode->IsTutorial())
		{
			if (MaldrithMusicAsset)
			{
				if (!bStartedBossMusic)
				{
					bStartedBossMusic = true;

					if (IsValid(GetAudioManager()))
					{
						if (ImpactSound)
						{
							GetAudioManager()->PlayMusic(MaldrithMusicAsset, true);
						}
					}
				}
			}
		}
	}
}

void ABSCharacterEnemyMaldrith::BeginPlay()
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
}

void ABSCharacterEnemyMaldrith::OnDeath()
{
	Super::OnDeath();

	if (BossHealthBarWidget)
	{
		BossHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ABSGameMode* GameMode = Cast<ABSGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (!GameMode->IsTutorial())
		{
			if (IsValid(GetAudioManager()))
			{
				GetAudioManager()->StopMusic();
			}
		}
	}
}

void ABSCharacterEnemyMaldrith::HitReaction(const AActor* Attacker, const EDamageType& DamageType)
{
}
