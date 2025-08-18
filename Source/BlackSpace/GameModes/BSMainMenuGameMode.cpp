// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/BSMainMenuGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "UI/BSMainMenuWidget.h"

ABSMainMenuGameMode::ABSMainMenuGameMode()
{
}

void ABSMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		UBSMainMenuWidget* MainMenuWidget = CreateWidget<UBSMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());
	}
}
