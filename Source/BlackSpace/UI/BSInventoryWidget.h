// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSInventoryWidget.generated.h"

class UGridPanel;
class UBSInventorySlotWidget;
class UTextBlock;
class UBSItemDescriptionWidget;
struct FInventorySlot;

UCLASS()
class BLACKSPACE_API UBSInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBSInventorySlotWidget> SlotWidget;

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UGridPanel> GridPanel;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> HoveredItemNameText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> HoveredItemMoreInfo;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSItemDescriptionWidget> WBPItemDescription;

// Tile Info
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> InventoryTiles;

public:
	UBSInventoryWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

public:
	void SetGrid();

protected:
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void OnInventoryUpdated(const TArray<FInventorySlot>& InventorySlotInfo);
	void OnMouseEnterToSlot(const FInventorySlot& InDescriptionSlot);
	void OnMouseLeaveFromSlot() const;

};
