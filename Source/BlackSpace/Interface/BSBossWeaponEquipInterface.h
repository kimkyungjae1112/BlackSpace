// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSBossWeaponEquipInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSBossWeaponEquipInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSBossWeaponEquipInterface
{
	GENERATED_BODY()

public:
	virtual void EquipWeapon() = 0;

	virtual void PlayEquipMontage() = 0;

	virtual AController* GetBossController() const = 0;
};
