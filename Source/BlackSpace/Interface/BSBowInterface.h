// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSBowInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSBowInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSBowInterface
{
	GENERATED_BODY()

public:
	virtual void PullString(const FVector& HandSocketLoc) = 0;
	virtual void ReleaseString() = 0;
};
