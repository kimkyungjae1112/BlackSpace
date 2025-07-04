// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSComboWindowInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSComboWindowInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSComboWindowInterface
{
	GENERATED_BODY()

public:
	virtual void EnableComboWindow() = 0;
	virtual void DisableComboWindow() = 0;
};
