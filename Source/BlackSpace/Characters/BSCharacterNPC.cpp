// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BSCharacterNPC.h"
#include "Components/BSDialogueComponent.h"

ABSCharacterNPC::ABSCharacterNPC()
{
	DialogueComp = CreateDefaultSubobject<UBSDialogueComponent>(TEXT("Dialogue Component"));
}

void ABSCharacterNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSCharacterNPC::StartDialogue()
{
	DialogueComp->StartDialogue();
}

void ABSCharacterNPC::NextDialogue()
{
	DialogueComp->NextDialogue();
}

void ABSCharacterNPC::SkipDialogue()
{
	DialogueComp->SkipDialogue();
}

