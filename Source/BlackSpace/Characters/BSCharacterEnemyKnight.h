// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BSCharacterEnemy.h"
#include "BSCharacterEnemyKnight.generated.h"

class UBSBossHealthBarWidget;

UCLASS()
class BLACKSPACE_API ABSCharacterEnemyKnight : public ABSCharacterEnemy
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
	TObjectPtr<USoundWave> KnightMusicAsset;

	UPROPERTY(EditAnywhere, Category = "Enemy | Sound")
	TObjectPtr<UAudioComponent> KnightMusicComp;

	bool bStartedBossMusic = false;

protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Set Value")
	int32 BlockingRate = 30;

	UPROPERTY(EditAnywhere, Category = "Enemy | Set Value")
	int32 ParryingAttackRate = 30;

	FTimerHandle BlockingDelayTimerHandle;

public:
	ABSCharacterEnemyKnight();

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* IBSCombatInterface Implement */
	virtual void EnemyBlocking() override;
	virtual void EnemyDodge() override;

	/* IBSEnemyInterface Implement */
	virtual void PostureAttacked(UAnimMontage* PostureAttackReactionMontage) override;

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
};
