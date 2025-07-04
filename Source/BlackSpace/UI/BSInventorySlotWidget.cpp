// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSInventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "UI/Action/BSInventoryDragDrop.h"
#include "UI/BSInventoryWidget.h"
#include "Equipments/BSWeapon.h"
#include "Components/BSInventoryComponent.h"

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
		ItemImage->SetBrushFromTexture(InventorySlot.Image);
		ItemImage->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
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

	TArray<UUserWidget*> InventoryWidgets;
	TSubclassOf<UBSInventoryWidget> InventoryWidgetClass = UBSInventoryWidget::StaticClass();

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), InventoryWidgets, InventoryWidgetClass, true);

	for (UUserWidget* Widget : InventoryWidgets)
	{
		UBSInventoryWidget* InventoryWidget = Cast<UBSInventoryWidget>(Widget);
		InventoryWidget->SetGrid();
		break;
	}
}

void UBSInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (InventorySlot.Quantity > 0)
	{
		TextName->SetText(InventorySlot.Name);
		TextQuantity->SetText(FText::AsNumber(InventorySlot.Quantity));
		ItemImage->SetBrushFromTexture(InventorySlot.Image);

		UBSInventorySlotWidget* InventorySlotWidget_DD = CreateWidget<UBSInventorySlotWidget>(GetWorld(), InventorySlotWidgetDragDrop);
		InventorySlotWidget_DD->SetInventorySlot(InventorySlot);
		InventorySlotWidget_DD->SetIndex(Index);
		InventorySlotWidget_DD->SetSlot();

		UBSInventoryDragDrop* DragDropOp = NewObject<UBSInventoryDragDrop>();
		DragDropOp->DefaultDragVisual = InventorySlotWidget_DD;
		DragDropOp->InventorySlot = InventorySlot;
		DragDropOp->Index = Index;

		OutOperation = DragDropOp;
	}
}

bool UBSInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool SuperResult = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return false;
}

void UBSInventorySlotWidget::RightClickForEquip()
{
	if (InventorySlot.Quantity > 0)
	{
		AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (Player == nullptr) return;

		FActorSpawnParameters Param;
		Param.Owner = Player;

		ABSWeapon* Weapon = GetWorld()->SpawnActor<ABSWeapon>(InventorySlot.ItemClass, Player->GetActorTransform(), Param);
		if (Weapon)
		{
			Weapon->EquipItem();
		}

		InventorySlot = FInventorySlot();
		SetSlot();

		if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
		{
			InventoryComp->RemoveToSlot(Index);
		}
	}
}
