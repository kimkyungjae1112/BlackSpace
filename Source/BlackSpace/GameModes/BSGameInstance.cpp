// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/BSGameInstance.h"
#include "UI/BSInventoryMenuWidget.h"

void UBSGameInstance::Shutdown()
{
	UBSInventoryMenuWidget::CurrentTabIndex = 1;

	Super::Shutdown();
}
