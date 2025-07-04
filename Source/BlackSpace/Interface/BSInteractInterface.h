// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSInteractInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSInteractInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* Interactor) = 0;
};
