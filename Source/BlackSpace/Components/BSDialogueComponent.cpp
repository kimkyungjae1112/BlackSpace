// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BSDialogueComponent.h"

UBSDialogueComponent::UBSDialogueComponent()
{

}


void UBSDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UBSDialogueComponent::StartDialogue()
{
	UE_LOG(LogTemp, Display, TEXT("Start Dialogue"));
}



