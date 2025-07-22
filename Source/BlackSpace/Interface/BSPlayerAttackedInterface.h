// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSPlayerAttackedInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSPlayerAttackedInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSPlayerAttackedInterface
{
	GENERATED_BODY()

public:
	virtual void GSwordSpecialAttackExecuted(UAnimMontage* Montage) = 0;
};
