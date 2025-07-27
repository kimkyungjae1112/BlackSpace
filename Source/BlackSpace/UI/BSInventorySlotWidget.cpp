// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSInventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

#include "UI/Action/BSInventoryDragDrop.h"
#include "UI/BSInventoryWidget.h"
#include "UI/BSInventoryMenuWidget.h"
#include "UI/BSMixtureWeaponWidget.h"
#include "Equipments/BSWeapon.h"
#include "Components/BSInventoryComponent.h"
#include "Components/BSCombatComponent.h"

UBSInventorySlotWidget::UBSInventorySlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBSInventorySlotWidget::SetSlot()
{
	if (InventorySlot.Quantity == 0)
	{
		TextName->SetText(FText::GetEmpty());
		TextQuantity->SetText(FText::GetEmpty());
		ItemImage->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
	}
	else
	{
		TextName->SetText(InventorySlot.Name);
		TextQuantity->SetText(FText::AsNumber(InventorySlot.Quantity));
		ItemImage->SetBrushFromTexture(InventorySlot.Image);
	}
}

FReply UBSInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		RightClickForEquip();
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UBSInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	if (UBSInventoryDragDrop* DragDropOp = Cast<UBSInventoryDragDrop>(InOperation))
	{
		SetSlotAtIndex(DragDropOp->Index, DragDropOp->InventorySlot);
	}
}

void UBSInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (InventorySlot.Quantity > 0)
	{
		UBSInventorySlotWidget* InventorySlotWidget_DD = CreateWidget<UBSInventorySlotWidget>(GetWorld(), InventorySlotWidgetDragDrop);
		InventorySlotWidget_DD->SetInventorySlot(InventorySlot);
		InventorySlotWidget_DD->SetIndex(Index);
		InventorySlotWidget_DD->SetSlot();

		UBSInventoryDragDrop* DragDropOp = NewObject<UBSInventoryDragDrop>();
		DragDropOp->DefaultDragVisual = InventorySlotWidget_DD;
		DragDropOp->InventorySlot = InventorySlot;
		DragDropOp->Index = Index;

		OutOperation = DragDropOp;

		SetSlotAtIndex(Index, FInventorySlot());
	}
}

bool UBSInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool SuperResult = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UBSInventoryDragDrop* DragDropOp = Cast<UBSInventoryDragDrop>(InOperation);
	if (DragDropOp == nullptr) return false;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC == nullptr) return false;

	UBSInventoryComponent* InventoryComp = PC->GetPawn()->GetComponentByClass<UBSInventoryComponent>();
	if (InventoryComp == nullptr) return false;

	const int32 FromIndex = DragDropOp->Index;
	const int32 ToIndex = this->Index;

	if (FromIndex == ToIndex)
	{
		return false;
	}

	InventoryComp->SetSlotAtIndex(FromIndex, DragDropOp->InventorySlot);
	InventoryComp->SwapSlot(FromIndex, ToIndex);

	return true;
}

void UBSInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (SelectedTexture && ItemSlot)
	{
		ItemSlot->SetBrushFromTexture(SelectedTexture);

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (UBSInventoryComponent* InventoryComp = PC->GetPawn()->GetComponentByClass<UBSInventoryComponent>())
			{
				InventoryComp->SetDescriptionSlot(InventorySlot);
			}
		}
	}
}

void UBSInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (NormalTexture && ItemSlot)
	{
		ItemSlot->SetBrushFromTexture(NormalTexture);

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (UBSInventoryComponent* InventoryComp = PC->GetPawn()->GetComponentByClass<UBSInventoryComponent>())
			{
				InventoryComp->SetDescriptionSlot(FInventorySlot());
			}
		}
	}
}

void UBSInventorySlotWidget::RightClickForEquip()
{
	if (InventorySlot.Quantity > 0)
	{
		APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (Player == nullptr) return;

		if (UBSInventoryMenuWidget::CurrentTabIndex == 2)
		{
			TArray<UUserWidget*> Widgets;
			TSubclassOf<UBSInventoryMenuWidget> InventoryWidgetClass = UBSInventoryMenuWidget::StaticClass();
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, InventoryWidgetClass, true);
			for (UUserWidget* Widget : Widgets)
			{
				UBSInventoryMenuWidget* InventoryMenuWidget = Cast<UBSInventoryMenuWidget>(Widget);
				
				InventoryMenuWidget->SetWeaponSlot(InventorySlot);
				// 아래 함수들이 줄줄이 달려있음
				// UBSMixtureWeaponWidget->SetWeaponSlot()
				// UBSMixtureMaterialWidget->SetWeaponSlot()
				
				if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
				{
					InventoryComp->RemoveToSlot(Index);
				}

				break;
			}
			return;
		}

		FActorSpawnParameters Param;
		Param.Owner = Player;

		FInventorySlot OldInventorySlot;
		if (UBSCombatComponent* CombatComp = Player->GetComponentByClass<UBSCombatComponent>())
		{
			if (CombatComp->CheckHasMainWeapon())
			{
				if (ABSWeapon* OldWeapon = CombatComp->GetMainWeapon())
				{
					OldInventorySlot = OldWeapon->GetInventoryInfo();
				}
			}
		}

		if (ABSWeapon* Weapon = GetWorld()->SpawnActor<ABSWeapon>(InventorySlot.ItemClass, Player->GetActorTransform(), Param))
		{
			Weapon->EquipItem();
		}

		if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
		{
			InventoryComp->RemoveToSlot(Index);

			// 우클릭을 통해 무기를 장착했는데, 이때 메인 무기가 장착되어 있었을 때
			if (UBSCombatComponent* CombatComp = Player->GetComponentByClass<UBSCombatComponent>())
			{
				if (CombatComp->CheckHasMainWeapon())
				{				
					InventoryComp->AddToSlot(OldInventorySlot, Index);
				}
			}
		}
	}
}

void UBSInventorySlotWidget::SetSlotAtIndex(int32 InIndex, const FInventorySlot& InInventorySlot) const
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (UBSInventoryComponent* InventoryComp = PC->GetPawn()->GetComponentByClass<UBSInventoryComponent>())
		{
			InventoryComp->SetSlotAtIndex(InIndex, InInventorySlot);
		}
	}
}
