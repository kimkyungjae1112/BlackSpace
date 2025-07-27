// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BSInventoryComponent.h"

#include "UI/BSPlayerHUDWidget.h"
#include "UI/BSInventoryMenuWidget.h"
#include "Player/BSPlayerController.h"
#include "Interface/BSPlayerHUDInterface.h"
#include "BSInventorySlot.h"

UBSInventoryComponent::UBSInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InventorySlots.SetNum(12);
}

void UBSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InventoryMenuWidget = CreateWidget<UBSInventoryMenuWidget>(GetWorld(), InventoryMenuWidgetClass);
	if (InventoryMenuWidget)
	{
		InventoryMenuWidget->AddToViewport();
		InventoryMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBSInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBSInventoryComponent::ToggleInventory()
{
	IBSPlayerHUDInterface* HUDInterface = Cast<IBSPlayerHUDInterface>(GetOwner());
	check(HUDInterface);

	ABSPlayerController* PC = Cast<ABSPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		if (InventoryMenuWidget->IsVisible())
		{
			InventoryMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			PC->SetInputModeGameOnly();
			
			if (UBSPlayerHUDWidget* HUDWidget = HUDInterface->GetHUDWidget())
			{
				HUDWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			InventoryMenuWidget->SetVisibility(ESlateVisibility::Visible);
			PC->SetInputModeUIAndGame();

			if (UBSPlayerHUDWidget* HUDWidget = HUDInterface->GetHUDWidget())
			{
				HUDWidget->SetVisibility(ESlateVisibility::Hidden);
			}
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

			if (OnInventoryUpdated.IsBound())
			{
				OnInventoryUpdated.Broadcast(InventorySlots);
			}

			break;
		}
	}
}

void UBSInventoryComponent::AddToSlot(const FInventorySlot& InventorySlot, int32 Index)
{
	if (Index >= 0 && InventorySlots.Num() > Index)
	{
		InventorySlots[Index] = InventorySlot;

		if (OnInventoryUpdated.IsBound())
		{
			OnInventoryUpdated.Broadcast(InventorySlots);
		}
	}
}

void UBSInventoryComponent::RemoveToSlot(const int32 Index)
{
	if (Index >= 0 && InventorySlots.Num() > Index)
	{
		InventorySlots[Index] = FInventorySlot();

		if (OnInventoryUpdated.IsBound())
		{
			OnInventoryUpdated.Broadcast(InventorySlots);
		}
	}
}

void UBSInventoryComponent::SwapSlot(int32 IndexA, int32 IndexB)
{
	if (!InventorySlots.IsValidIndex(IndexA) || !InventorySlots.IsValidIndex(IndexB))
	{
		return;
	}

	InventorySlots.Swap(IndexA, IndexB);

	if (OnInventoryUpdated.IsBound())
	{
		OnInventoryUpdated.Broadcast(InventorySlots);
	}
}

void UBSInventoryComponent::SetSlotAtIndex(int32 Index, const FInventorySlot& SlotData)
{
	if (InventorySlots.IsValidIndex(Index))
	{
		InventorySlots[Index] = SlotData;

		OnInventoryUpdated.Broadcast(InventorySlots);
	}
}

void UBSInventoryComponent::SetDescriptionSlot(const FInventorySlot& InDescriptionSlot)
{
	DescriptionSlot = InDescriptionSlot;

	if (OnMouseEnterToSlot.IsBound())
	{
		OnMouseEnterToSlot.Broadcast(DescriptionSlot.Name, DescriptionSlot.Description);
	}
}

void UBSInventoryComponent::InventoryLeftPage()
{
	if (InventoryMenuWidget->IsVisible())
	{
		InventoryMenuWidget->OnLeftTab();
	}
}

void UBSInventoryComponent::InventoryRightPage()
{
	if (InventoryMenuWidget->IsVisible())
	{
		InventoryMenuWidget->OnRightTab();
	}
}

