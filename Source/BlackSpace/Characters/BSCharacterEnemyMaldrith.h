// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BSCharacterEnemy.h"
#include "Interface/BSBossWeaponEquipInterface.h"
#include "BSCharacterEnemyMaldrith.generated.h"

class UBSBossHealthBarWidget;
class USoundCue;

UCLASS()
class BLACKSPACE_API ABSCharacterEnemyMaldrith 
	: public ABSCharacterEnemy
	, public IBSBossWeaponEquipInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Name")
	FText BossName;

// UI
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | UI")
	TSubclassOf<UBSBossHealthBarWidget> BossHealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | UI")
	TObjectPtr<UBSBossHealthBarWidget> BossHealthBarWidget;

protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Sound")
	TObjectPtr<USoundCue> MaldrithMusicAsset;

	bool bStartedBossMusic = false;

// Animation
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Animation")
	TObjectPtr<UAnimMontage> EquipMontage;

public:
	ABSCharacterEnemyMaldrith();

public:
	/* IBSEnemyInterface Implement */
	virtual void PostureAttacked(UAnimMontage* PostureAttackReactionMontage) override;

	/* IBSBossWeaponEquipInterface Implement */
	virtual void EquipWeapon() override;
	virtual void PlayEquipMontage() override;

public:
	virtual void SeesTarget(AActor* InTargetActor) override;

protected:
	virtual void BeginPlay() override;

// 죽음 및 피격 관련
protected:
	virtual void OnDeath() override;
	virtual void HitReaction(const AActor* Attacker, const EDamageType& DamageType) override;
};
