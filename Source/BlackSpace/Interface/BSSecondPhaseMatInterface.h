// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSSecondPhaseMatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSSecondPhaseMatInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSSecondPhaseMatInterface
{
	GENERATED_BODY()

public:
	virtual void LoadBodyMeshMaterial() const = 0;
};
