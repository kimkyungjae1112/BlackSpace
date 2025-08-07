// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BSDialogueEndInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBSDialogueEndInterface : public UInterface
{
	GENERATED_BODY()
};

class BLACKSPACE_API IBSDialogueEndInterface
{
	GENERATED_BODY()

public:
	virtual void EndDialogue() = 0;
	
	virtual void OptionDialogue() = 0;

	virtual void EndOptionDialogue() = 0;
};
