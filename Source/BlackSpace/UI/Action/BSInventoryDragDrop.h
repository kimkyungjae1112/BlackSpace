// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "BSInventorySlot.h"
#include "BSInventoryDragDrop.generated.h"

UCLASS()
class BLACKSPACE_API UBSInventoryDragDrop : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "DranDrop")
	int32 Index;

	UPROPERTY(VisibleAnywhere, Category = "DranDrop")
	FInventorySlot InventorySlot;

public:
	UBSInventoryDragDrop();
};
