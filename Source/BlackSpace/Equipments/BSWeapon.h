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
class USoundCue;
class UCameraShakeBase;

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

	UPROPERTY(VisibleAnywhere, Category = "Equipment | Component")
	TObjectPtr<UBSWeaponCollisionComponent> SecondaryCollisionComp;

// Stat
protected:
	UPROPERTY(EditAnywhere, Category = "Equipment | Stamina")
	TMap<FGameplayTag, float> StaminaCosts;

	UPROPERTY(EditAnywhere, Category = "Equipment | Damage")
	TMap<EWeaponGrade, float> DamageMultiplierByGrade;

	UPROPERTY(EditAnywhere, Category = "Equipment | Damage")
	TMap<FGameplayTag, float> DamageMultiplierMap;

	UPROPERTY(EditAnywhere, Category = "Equipment | Damage")
	float BaseDamage = 10.f;

// Sound
protected:
	UPROPERTY(EditAnywhere, Category = "Equipment | Sound")
	TObjectPtr<USoundCue> SwingSoundCue;

// Camera Shake
protected:
	UPROPERTY(EditAnywhere, Category = "Equipment | Camera Shake")
	TSubclassOf<UCameraShakeBase> SwingCameraShake;

public:
	ABSWeapon();

	FORCEINLINE FName GetEquipSocket() const { return EquipSocket; }
	FORCEINLINE UBSWeaponCollisionComponent* GetCollision() const { return CollisionComp; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE float GetWeaponDamage() const { return BaseDamage; }

public:
	virtual void EquipItem() override;

	virtual void UnequipItem() override;

	// 무기 공격력 설정
	virtual	void OnceCalledSetWeaponDamage() override;

	void OnUpdateWeaponType();

public:
	UAnimMontage* GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index = 0) const;
	UAnimMontage* GetRandomMontageForTag(const FGameplayTag& GroupTag) const;
	UAnimMontage* GetHitReactMontage(const AActor* Attacker) const;

	float GetStaminaCost(const FGameplayTag& InAttackType) const;
	float GetAttackDamage() const;

	void Drop();

	// Sound
	void PlaySwingSound();
	
	// Camera Shake
	void PlaySwordSwingCameraShake(float Scale = 1.f) const;

public:
	void ActivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType);
	void DeactivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType);

protected:
	void OnHitActor(const FHitResult& HitResult);

};
