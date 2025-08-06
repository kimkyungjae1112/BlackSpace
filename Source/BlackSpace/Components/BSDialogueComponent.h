// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BSDialogueComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLACKSPACE_API UBSDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBSDialogueComponent();

protected:
	virtual void BeginPlay() override;

public:
	void StartDialogue();
		
};
