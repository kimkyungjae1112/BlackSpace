// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BSInventoryComponent.generated.h"

class UBSInventoryWidget;
struct FInventorySlot;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLACKSPACE_API UBSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<FInventorySlot> InventorySlots;

// UI
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UBSInventoryWidget> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UBSInventoryWidget> InventoryWidget;

public:	
	UBSInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE TArray<FInventorySlot> GetInventoryTiles() const { return InventorySlots; }

public:
	void ToggleInventory();

	void AddToSlot(const FInventorySlot& InventorySlot);

	void RemoveToSlot(const int32 Index);
};
