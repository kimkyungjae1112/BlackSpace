// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSInventoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/GridPanel.h"
#include "Components/TextBlock.h"

#include "Components/BSInventoryComponent.h"
#include "UI/BSInventorySlotWidget.h"
#include "UI/Action/BSInventoryDragDrop.h"
#include "UI/BSItemDescriptionWidget.h"

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
			InventoryComp->OnInventoryUpdated.AddUObject(this, &ThisClass::OnInventoryUpdated);
			InventoryComp->OnMouseEnterToSlot.AddUObject(this, &ThisClass::OnMouseEnterToSlot);
			InventoryComp->OnMouseLeaveFromSlot.AddUObject(this, &ThisClass::OnMouseLeaveFromSlot);
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

		GridPanel->AddChildToGrid(TileWidget, i / 3, i % 3);
	}
}

void UBSInventoryWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	// 아이템 DragDrop 취소시 데이터 되돌아오는 로직
	// 근데 문제가 있음 
	// NativeOnDrop 까지

	if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
		{
			if (UBSInventoryDragDrop* DragDropOp = Cast<UBSInventoryDragDrop>(InOperation))
			{
				InventoryComp->SetSlotAtIndex(DragDropOp->Index, DragDropOp->InventorySlot);
			}
		}
	}
}

bool UBSInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool SuperResult = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return false;
}

void UBSInventoryWidget::OnInventoryUpdated(const TArray<FInventorySlot>& InventorySlotInfo)
{
	InventoryTiles = InventorySlotInfo;
	SetGrid();
}

void UBSInventoryWidget::OnMouseEnterToSlot(const FInventorySlot& InDescriptionSlot)
{
	HoveredItemNameText->SetText(InDescriptionSlot.Name);
	HoveredItemMoreInfo->SetText(InDescriptionSlot.Description);

	WBPItemDescription->SetDescription(InDescriptionSlot);
}

void UBSInventoryWidget::OnMouseLeaveFromSlot() const
{
	WBPItemDescription->UnsetDescription();
}

