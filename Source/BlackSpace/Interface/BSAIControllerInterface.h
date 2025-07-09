// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSAIControllerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSAIControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSAIControllerInterface
{
	GENERATED_BODY()

public:
	virtual bool IsDetectedPlayer() const = 0;
};
