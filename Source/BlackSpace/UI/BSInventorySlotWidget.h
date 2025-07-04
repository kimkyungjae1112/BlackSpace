// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSInventorySlot.h"
#include "BSInventorySlotWidget.generated.h"

class UTextBlock;
class UBorder;

UCLASS()
class BLACKSPACE_API UBSInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
// UI
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> TextName;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> TextQuantity;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBorder> ItemImage;

// Slot Info
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Slot Info")
	FInventorySlot InventorySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Info")
	int32 Index;

// DragDrop
protected:
	UPROPERTY(EditAnywhere, Category = "DragDrop")
	TSubclassOf<UBSInventorySlotWidget> InventorySlotWidgetDragDrop;

public:
	UBSInventorySlotWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FORCEINLINE void SetInventorySlot(const FInventorySlot& InInventoryTile) { InventorySlot = InInventoryTile; }
	FORCEINLINE void SetIndex(int32 InIndex) { Index = InIndex; }

protected:
	virtual void NativeConstruct() override;

public:
	void SetSlot();

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	void RightClickForEquip();
};
