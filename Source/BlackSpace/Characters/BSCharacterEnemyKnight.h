// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BSCharacterEnemy.h"
#include "Interface/BSSecondPhaseMatInterface.h"
#include "BSCharacterEnemyKnight.generated.h"

class UBSBossHealthBarWidget;
class UMotionWarpingComponent;
class USoundCue;
class ABSWeapon;

UCLASS()
class BLACKSPACE_API ABSCharacterEnemyKnight 
	: public ABSCharacterEnemy
	, public IBSSecondPhaseMatInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Name")
	FText BossName;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UMotionWarpingComponent> MotionWarp;

// UI
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | UI")
	TSubclassOf<UBSBossHealthBarWidget> BossHealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | UI")
	TObjectPtr<UBSBossHealthBarWidget> BossHealthBarWidget;

// Sound
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Sound")
	TObjectPtr<USoundCue> KnightMusicAsset;

	bool bStartedBossMusic = false;

// Weapon
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Weapon")
	TSubclassOf<ABSWeapon> GreateSwordWeaponClass;

protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Set Value")
	int32 BlockingRate = 30;

	UPROPERTY(EditAnywhere, Category = "Enemy | Set Value")
	int32 ParryingAttackRate = 30;

	UPROPERTY(EditAnywhere, Category = "Enemy | Set Value")
	int32 DodgeRate = 30;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Set Value")
	bool bIsActiveSecondPhase = false;

	FTimerHandle BlockingDelayTimerHandle;

protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Montage")
	TObjectPtr<UAnimMontage> SecondPhaseMontage;
	
public:
	ABSCharacterEnemyKnight();

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* IBSCombatInterface Implement */
	virtual void EnemyBlocking() override;
	virtual void EnemyDodge() override;
	virtual void PerformAttack(const FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) override;

	/* IBSEnemyInterface Implement */
	virtual void PostureAttacked(UAnimMontage* PostureAttackReactionMontage) override;

	/* IBSSecondePhaseInterface Implement */
	virtual void LoadBodyMeshMaterial() const override;

public:
	virtual void SeesTarget(AActor* InTargetActor) override;

protected:
	virtual void BeginPlay() override;

// 죽음 및 피격 관련
protected:
	virtual void OnDeath() override;
	virtual void HitReaction(const AActor* Attacker, const EDamageType& DamageType) override;

// 방어
protected:
	bool CanBlocking() const;

	UFUNCTION()
	void BlockingEnableAction();

// 회피
protected:
	bool CanDodge() const;

// Motion Warping
protected:
	void RangedAttackMotionWarping() const;

// 무기 교체
protected:
	void ChangeWeapon();
};
