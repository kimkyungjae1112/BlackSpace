// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSInventoryMenuWidget.h"

UBSInventoryMenuWidget::UBSInventoryMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSInventoryMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBSInventoryMenuWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

bool UBSInventoryMenuWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool SuperResult = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return false;
}

