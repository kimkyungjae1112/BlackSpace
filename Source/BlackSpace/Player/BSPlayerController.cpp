// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BSPlayerController.h"

ABSPlayerController::ABSPlayerController()
{
}

void ABSPlayerController::SetInputModeUIAndGame()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI GameAndUI;
	SetInputMode(GameAndUI);
}

void ABSPlayerController::SetInputModeGameOnly()
{
	bShowMouseCursor = false;
	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);
}
