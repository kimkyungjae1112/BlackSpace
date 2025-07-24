// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSTargetingInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSTargetingInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSTargetingInterface
{
	GENERATED_BODY()

public:
	virtual void OnTargeted(bool bTargeted) = 0;

	virtual bool CanBeTargeted() = 0;
};
