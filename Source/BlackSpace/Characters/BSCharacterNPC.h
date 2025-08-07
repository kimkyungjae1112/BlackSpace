// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/BSDialogueInterface.h"
#include "BSCharacterNPC.generated.h"

class UBSDialogueComponent;

UCLASS()
class BLACKSPACE_API ABSCharacterNPC 
	: public ACharacter
	, public IBSDialogueInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "NPC | Component")
	TObjectPtr<UBSDialogueComponent> DialogueComp;

public:
	ABSCharacterNPC();

protected:
	virtual void BeginPlay() override;

public:
	/* IBSDialogueInterface Implement */
	virtual void StartDialogue() override;
	virtual void NextDialogue() override;
	virtual void SkipDialogue() override;

public:
};
