// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSInventoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/GridPanel.h"

#include "Components/BSInventoryComponent.h"
#include "UI/BSInventorySlotWidget.h"

UBSInventoryWidget::UBSInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
		{
			InventoryTiles = InventoryComp->GetInventoryTiles(); 
		}
	}

	SetGrid();
}

void UBSInventoryWidget::SetGrid()
{
	GridPanel->ClearChildren();
	for (int32 i = 0; i < InventoryTiles.Num(); ++i)
	{
		UBSInventorySlotWidget* TileWidget = CreateWidget<UBSInventorySlotWidget>(GetWorld(), SlotWidget);
		TileWidget->SetInventorySlot(InventoryTiles[i]);
		TileWidget->SetIndex(i);
		TileWidget->SetSlot();

		GridPanel->AddChildToGrid(TileWidget, i / 4, i % 4);
	}
}
