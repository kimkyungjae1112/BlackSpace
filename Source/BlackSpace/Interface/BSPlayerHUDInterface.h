// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSPlayerHUDInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSPlayerHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSPlayerHUDInterface
{
	GENERATED_BODY()

public:
	virtual class UBSPlayerHUDWidget* GetHUDWidget() const = 0;
};
