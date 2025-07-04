// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BSInventoryComponent.h"

#include "UI/BSInventoryWidget.h"
#include "Player/BSPlayerController.h"
#include "BSInventorySlot.h"

UBSInventoryComponent::UBSInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InventorySlots.SetNum(16);
}

void UBSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InventoryWidget = CreateWidget<UBSInventoryWidget>(GetWorld(), InventoryWidgetClass);
}

void UBSInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBSInventoryComponent::ToggleInventory()
{
	ABSPlayerController* PC = Cast<ABSPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		if (InventoryWidget)
		{
			if (InventoryWidget->IsInViewport())
			{
				InventoryWidget->RemoveFromParent();
				PC->SetInputModeGameOnly();
			}
			else
			{
				InventoryWidget->AddToViewport();
				PC->SetInputModeUIAndGame();
			}
		}
		else
		{
			InventoryWidget = CreateWidget<UBSInventoryWidget>(GetWorld(), InventoryWidgetClass);
			InventoryWidget->AddToViewport();
			PC->SetInputModeUIAndGame();
		}
	}
}

void UBSInventoryComponent::AddToSlot(const FInventorySlot& InventorySlot)
{
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (InventorySlots[i].Quantity == 0)
		{
			InventorySlots[i] = InventorySlot;
			break;
		}
	}
}

void UBSInventoryComponent::RemoveToSlot(const int32 Index)
{
	if (Index >= 0 && InventorySlots.Num() > Index)
	{
		InventorySlots[Index] = FInventorySlot();
	}
}

