// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BSCharacterBase.h"
#include "BSDefine.h"
#include "BSCharacterEnemy.generated.h"

class UBSStateComponent;
class UBSAttributeComponent;
class UBSCombatComponent;
class UBSRotationComponent;
class UWidgetComponent;
class USoundCue;
class ATargetPoint;
class ABSWeapon;

UCLASS()
class BLACKSPACE_API ABSCharacterEnemy : public ABSCharacterBase
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

public:
	ABSCharacterEnemy();

	FORCEINLINE ATargetPoint* GetPatrolPoint() const { return PatrolPoints.Num() >= (PatrolIndex + 1) ? PatrolPoints[PatrolIndex] : nullptr; }
	FORCEINLINE void IncrementPatrolIndex() { PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num(); }

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	/* IBSCombatInterface Implement */
	virtual void ActivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType) override;
	virtual void PerformAttack(const FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) override;
	virtual void Parried() override;

public:
	void ToggleHealthBarVisibility(bool bVisibility) const;

protected:
	virtual void OnDeath() override;
	virtual void ImpactEffect(const FVector& Location) override;
	virtual void HitReaction(const AActor* Attacker) override;

	void OnChangedAttribute(const EAttributeType& AttributeType, float InRatio);
	void SetupAttribute();
};
