// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSBowFireInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSBowFireInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSBowFireInterface
{
	GENERATED_BODY()

public:
	virtual void BowFireFinished() = 0;
};
