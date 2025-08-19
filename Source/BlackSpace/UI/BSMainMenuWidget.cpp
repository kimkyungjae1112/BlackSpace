// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSMainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "GameModes/BSGameInstance.h"

void UBSMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ThisClass::GameStart);
	}

	if (TutorialButton)
	{
		TutorialButton->OnClicked.AddDynamic(this, &ThisClass::TutorialStart);
	}

	if (SettingButton)
	{
		SettingButton->OnClicked.AddDynamic(this, &ThisClass::SettingStart);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::ExitStart);
	}
}

void UBSMainMenuWidget::GameStart() 
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Gothic_Cathedral"));
}

void UBSMainMenuWidget::TutorialStart() 
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("ThirdPersonMap"));
}

void UBSMainMenuWidget::SettingStart() 
{

}

void UBSMainMenuWidget::ExitStart() 
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
