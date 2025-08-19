// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSPauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

#include "GameModes/BSGameInstance.h"

void UBSPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &ThisClass::ResumeButtonHandle);
	}

	if (SettingButton)
	{
		SettingButton->OnClicked.AddDynamic(this, &ThisClass::SettingButtonHandle);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::MainMenuButtonHandle);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitButtonHandle);
	}

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
	}
}

void UBSPauseMenuWidget::ResumeButtonHandle()
{
	if (IsInViewport())
	{
		RemoveFromParent();
	}

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
}

void UBSPauseMenuWidget::SettingButtonHandle()
{
}

void UBSPauseMenuWidget::MainMenuButtonHandle()
{
	if (UBSGameInstance* GI = GetGameInstance<UBSGameInstance>())
	{
		GI->ShowLoadingScreen(3.f);
	}

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenuMap"));
}

void UBSPauseMenuWidget::QuitButtonHandle()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
