// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/BSGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void ABSGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
}
