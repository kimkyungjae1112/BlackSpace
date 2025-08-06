// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BSInventorySlot.h"
#include "BSInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const TArray<FInventorySlot>&)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMouseEnterToSlot, const FInventorySlot&)
DECLARE_MULTICAST_DELEGATE(FOnMouseLeaveFromSlot)

class UBSInventoryMenuWidget;
class ABSWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLACKSPACE_API UBSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/* 인벤토리 내에서 아이템이 변경되었을 때 호출되는 델리게이트 */
	FOnInventoryUpdated OnInventoryUpdated;

	/* 인벤토리 슬롯 위로 마우스를 올렸을 때 호출되는 델리게이트 */
	FOnMouseEnterToSlot OnMouseEnterToSlot;

	/* 인벤토리 슬롯에서 마우스가 나갔을 때 호출되는 델리게이트 */
	FOnMouseLeaveFromSlot OnMouseLeaveFromSlot;

protected:
	UPROPERTY(EditAnywhere, Category = "Inventory | Info")
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(EditAnywhere, Category = "Inventory | Weapon")
	TArray<ABSWeapon*> Weapons;

	FInventorySlot DescriptionSlot;

// UI
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UBSInventoryMenuWidget> InventoryMenuWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UBSInventoryMenuWidget> InventoryMenuWidget;

public:	
	UBSInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE TArray<FInventorySlot> GetInventoryTiles() const { return InventorySlots; }

	FORCEINLINE FInventorySlot GetDescriptionSlot() const { return DescriptionSlot; }
	

public:
	void ToggleInventory();

	void AddToSlot(const FInventorySlot& InventorySlot);
	void AddToSlot(ABSWeapon* InWeapon, int32 Index);
	void AddToSlot(ABSWeapon* InWeapon);
	void AddToSlot(int32 Index);

	void RemoveToSlot(const int32 Index);
	void RemoveWeapon(const int32 Index);

	void EquipFromInventory(const int32 Index) const;

	void SwapSlot(int32 IndexA, int32 IndexB);

	void SetSlotAtIndex(int32 Index, const FInventorySlot& SlotData);
	
	void SetDescriptionSlot(const FInventorySlot& InDescriptionSlot);

	void UnsetDescriptionSlot() const;

	void InventoryLeftPage();

	void InventoryRightPage();
};
