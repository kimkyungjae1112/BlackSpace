// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BSPlayerController.h"
#include "Camera/CameraShakeBase.h"

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

void ABSPlayerController::SetInputModeUIOnly()
{
	bShowMouseCursor = true;
	FInputModeUIOnly UIOnly;
	SetInputMode(UIOnly);
}