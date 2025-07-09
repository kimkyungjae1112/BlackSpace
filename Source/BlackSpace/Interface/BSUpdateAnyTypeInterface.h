// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSDefine.h"
#include "BSUpdateAnyTypeInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSUpdateAnyTypeInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSUpdateAnyTypeInterface
{
	GENERATED_BODY()

public:
	virtual void UpdateWeaponType(const EWeaponType& InWeaponType) = 0;
	virtual void UpdateBlcokingState(const bool bBlockingState) = 0;
};
