// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSDefine.h"
#include "BSCombatInterface.generated.h"

struct FGameplayTag;

UINTERFACE(MinimalAPI)
class UBSCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class BLACKSPACE_API IBSCombatInterface
{
	GENERATED_BODY()

public:
	virtual void ActivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType) = 0;

	virtual void DeactivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType) = 0;

	virtual void PerformAttack(const FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) {}

	virtual void Parried() {}
};
