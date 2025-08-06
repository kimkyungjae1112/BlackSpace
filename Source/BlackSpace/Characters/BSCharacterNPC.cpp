// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BSCharacterNPC.h"
#include "Components/BSDialogueComponent.h"

ABSCharacterNPC::ABSCharacterNPC()
{
	DialogueComponent = CreateDefaultSubobject<UBSDialogueComponent>(TEXT("Dialogue Component"));
}

void ABSCharacterNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSCharacterNPC::StartDialogue()
{
	DialogueComponent->StartDialogue();
}

