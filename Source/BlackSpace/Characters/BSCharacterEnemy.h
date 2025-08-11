// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BSCharacterBase.h"
#include "BSDefine.h"
#include "Interface/BSEnemyInterface.h"
#include "Interface/BSTargetingInterface.h"
#include "BSCharacterEnemy.generated.h"

class UBSStateComponent;
class UBSAttributeComponent;
class UBSCombatComponent;
class UBSRotationComponent;
class UWidgetComponent;
class USphereComponent;
class USoundCue;
class ATargetPoint;
class ABSWeapon;

UCLASS()
class BLACKSPACE_API ABSCharacterEnemy
	: public ABSCharacterBase
	, public IBSEnemyInterface
	, public IBSTargetingInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UBSStateComponent> StateComp;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UBSAttributeComponent> AttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UBSCombatComponent> CombatComp;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UBSRotationComponent> RotationComp;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UWidgetComponent> BackAttackWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UWidgetComponent> PostureWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UWidgetComponent> PostureAttackWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<USphereComponent> TargetingSphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UWidgetComponent> LockOnWidgetComp;

// AI
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | AI")
	TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | AI")
	int32 PatrolIndex = 0;

// Weapon
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Weapon")
	TSubclassOf<ABSWeapon> DefaultWeaponClass;

protected:
	FTimerHandle ParriedDelayTimerHandle;
	FTimerHandle StunnedDelayTimerHandle;

protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Set Value")
	bool bRootMotionTestFlag = false;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Set Value")
	bool bEnabledBlocking = false;

	UPROPERTY(EditAnywhere, Category = "Enemy | Set Value")
	bool bTakeDamageUsage = false;

	UPROPERTY(EditAnywhere, Category = "Enemy | Set Value")
	bool bUnstoppable = false;

	UPROPERTY(EditAnywhere, Category = "Enemy | Set Value")
	int32 StunnedRate = 0;

// Material
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Material")
	TObjectPtr<UMaterialInstance> OutlineMaterial;

public:
	ABSCharacterEnemy();

	FORCEINLINE ATargetPoint* GetPatrolPoint() const { return PatrolPoints.Num() >= (PatrolIndex + 1) ? PatrolPoints[PatrolIndex] : nullptr; }
	FORCEINLINE void IncrementPatrolIndex() { PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num(); }

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/* IBSCombatInterface Implement */
	virtual void ActivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType) override;
	virtual void PerformAttack(const FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) override;
	virtual void Parried() override;
	virtual void EnemyBlocking() override;
	virtual void EnemyDodge() override;

	/* IBSEnemyInterface Implement */
	virtual void ToggleBackAttackWidgetVisibility(const bool bShouldBackAttack) const override;
	virtual void BackAttacked(UAnimMontage* BackAttackReactionMontage) override;
	virtual bool IsEnabledPostureAttack() const override;
	virtual void TogglePostureAttackWidgetVisibility(const bool bShouldPostureAttack) const override;
	virtual void PostureAttacked(UAnimMontage* PostureAttackReactionMontage) override;

	/* IBSTargeting Implement */
	virtual void OnTargeted(bool bTargeted) override;
	virtual bool CanBeTargeted() override;

public:
	virtual void SeesTarget(AActor* InTargetActor);
	void ToggleHealthBarVisibility(bool bVisibility) const;

// 죽음 및 피격 관련
protected:
	virtual void OnDeath() override;
	void SetDeathState();
	virtual void ImpactEffect(const FVector& Location) override;
	void BlockImpactEffect(const FVector& Location);
	
	virtual void HitReaction(const AActor* Attacker, const EDamageType& DamageType) override;
	
	void OnChangedAttribute(const EAttributeType& AttributeType, float InRatio);
	void SetupAttribute();
	
	void OnPosture();

// 방어
protected:
	bool IsBlockingState() const;
};
