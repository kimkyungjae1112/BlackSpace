// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BSInventoryComponent.h"

#include "UI/BSPlayerHUDWidget.h"
#include "UI/BSInventoryMenuWidget.h"
#include "Player/BSPlayerController.h"
#include "Interface/BSPlayerHUDInterface.h"
#include "Equipments/BSWeapon.h"
#include "BSInventorySlot.h"

UBSInventoryComponent::UBSInventoryComponent()
{
	InventorySlots.SetNum(12);
	Weapons.SetNum(12);
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
			FActorSpawnParameters Param;
			Param.Owner = GetOwner();

			ABSWeapon* Weapon = GetWorld()->SpawnActor<ABSWeapon>(InventorySlot.ItemClass, GetOwner()->GetActorTransform(), Param);
			Weapon->SetInventoryInfo(InventorySlot);
			Weapon->OnceCalledSetWeaponDamage();
			Weapon->SetActorHiddenInGame(true);
			Weapons[i] = Weapon;

			InventorySlots[i] = Weapon->GetInventoryInfo();

			if (OnInventoryUpdated.IsBound())
			{
				OnInventoryUpdated.Broadcast(InventorySlots);
			}

			break;
		}
	}
}

void UBSInventoryComponent::AddToSlot(ABSWeapon* InWeapon, int32 Index)
{
	if (Index >= 0 && InventorySlots.Num() > Index)
	{
		if (InWeapon != nullptr)
		{
			InventorySlots[Index] = InWeapon->GetInventoryInfo();
		}

		Weapons[Index] = InWeapon;

		if (OnInventoryUpdated.IsBound())
		{
			OnInventoryUpdated.Broadcast(InventorySlots);
		}
	}
}

void UBSInventoryComponent::AddToSlot(ABSWeapon* InWeapon)
{
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (InventorySlots[i].Quantity == 0 && InWeapon)
		{
			InventorySlots[i] = InWeapon->GetInventoryInfo();

			Weapons[i] = InWeapon;

			if (OnInventoryUpdated.IsBound())
			{
				OnInventoryUpdated.Broadcast(InventorySlots);
			}

			break;
		}
	}
}

void UBSInventoryComponent::AddToSlot(int32 Index)
{
	if (InventorySlots[Index].Quantity == 0)
	{
		InventorySlots[Index] = Weapons[Index]->GetInventoryInfo();

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

void UBSInventoryComponent::RemoveWeapon(const int32 Index)
{
	if (Weapons.IsValidIndex(Index))
	{
		Weapons[Index]->Destroy();
		Weapons[Index] = nullptr;
	}
}

void UBSInventoryComponent::EquipFromInventory(const int32 Index) const
{
	if (Weapons.IsValidIndex(Index))
	{
		if (Weapons[Index])
		{
			Weapons[Index]->EquipItem();
			Weapons[Index]->SetActorHiddenInGame(false);
		}
	}
}

void UBSInventoryComponent::SwapSlot(int32 IndexA, int32 IndexB)
{
	if (!InventorySlots.IsValidIndex(IndexA) || !InventorySlots.IsValidIndex(IndexB))
	{
		return;
	}

	if (!Weapons.IsValidIndex(IndexA) || !Weapons.IsValidIndex(IndexB))
	{
		return;
	}

	InventorySlots.Swap(IndexA, IndexB);

	Weapons.Swap(IndexA, IndexB);

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
		OnMouseEnterToSlot.Broadcast(InDescriptionSlot);
	}
}

void UBSInventoryComponent::UnsetDescriptionSlot() const
{
	if (OnMouseLeaveFromSlot.IsBound())
	{
		OnMouseLeaveFromSlot.Broadcast();
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

