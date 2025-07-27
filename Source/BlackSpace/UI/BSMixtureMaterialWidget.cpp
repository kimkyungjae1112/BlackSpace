// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSMixtureMaterialWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "UI/BSInventoryMenuWidget.h"
#include "Components/BSInventoryComponent.h"
#include "BSInventorySlot.h"

UBSMixtureMaterialWidget::UBSMixtureMaterialWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSMixtureMaterialWidget::SetWeaponSlot(const FInventorySlot& InInventorySlot)
{
	bHasWeaponSlot = true;
	InventorySlot = InInventorySlot;
	WeaponName->SetText(InInventorySlot.Name);
	WeaponImage->SetBrushFromTexture(InInventorySlot.Image);
}

void UBSMixtureMaterialWidget::UnsetWeaponSlot()
{
	bHasWeaponSlot = false;
	InventorySlot = FInventorySlot();
	WeaponName->SetText(InventorySlot.Name);
	WeaponImage->SetBrushFromTexture(InventorySlot.Image);
}

void UBSMixtureMaterialWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UBSMixtureMaterialWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		RightClickForRemove();
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UBSMixtureMaterialWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

bool UBSMixtureMaterialWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool SuperResult = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return false;
}

void UBSMixtureMaterialWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (Border)
	{
		Border->SetBrushFromTexture(SelectedTexture);
	}
}

void UBSMixtureMaterialWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (Border)
	{
		Border->SetBrushFromTexture(NormalTexture);
	}
}

void UBSMixtureMaterialWidget::RightClickForRemove()
{
	if (InventorySlot.Quantity > 0)
	{
		if (APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
			{
				InventoryComp->AddToSlot(InventorySlot);

				UnsetWeaponSlot();
			}
		}
	}
}