// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/BSEquipmentBase.h"
#include "GameplayTagContainer.h"
#include "BSDefine.h"
#include "BSWeapon.generated.h"

struct FInventorySlot;

class UBSCombatComponent;
class UBSWeaponCollisionComponent;
class UBSMontageActionData;

UCLASS()
class BLACKSPACE_API ABSWeapon : public ABSEquipmentBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Equipment | Socket")
	FName EquipSocket;

	UPROPERTY(EditAnywhere, Category = "Equipment | Socket")
	FName UnequipSocket;

	UPROPERTY(EditAnywhere, Category = "Equipment | Montage")
	TObjectPtr<UBSMontageActionData> MontageData;

	UPROPERTY(EditAnywhere, Category = "Equipment | Type")
	EWeaponType WeaponType = EWeaponType::Sword;

// Component
protected:
	UPROPERTY(VisibleAnywhere, Category = "Equipment | Component")
	TObjectPtr<UBSCombatComponent> CombatComp;

	UPROPERTY(VisibleAnywhere, Category = "Equipment | Component")
	TObjectPtr<UBSWeaponCollisionComponent> CollisionComp;

// Stat
protected:
	UPROPERTY(EditAnywhere, Category = "Equipment | Stamina")
	TMap<FGameplayTag, float> StaminaCosts;

	UPROPERTY(EditAnywhere, Category = "Equipment | Damage")
	float BaseDamage = 15.f;

public:
	ABSWeapon();

	FORCEINLINE UBSWeaponCollisionComponent* GetCollision() const { return CollisionComp; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

public:
	virtual void EquipItem() override;

	virtual void UnequipItem() override;

	void OnUpdateWeaponType();

public:
	UAnimMontage* GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index = 0) const;

	float GetStaminaCost(const FGameplayTag& InAttackType) const;
	float GetAttackDamage() const;

protected:
	void OnHitActor(const FHitResult& HitResult);

};
