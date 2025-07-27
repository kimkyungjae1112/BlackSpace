// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSInventorySlot.h"
#include "BSMixtureMaterialWidget.generated.h"

class UTextBlock;
class UImage;
class UBorder;

UCLASS()
class BLACKSPACE_API UBSMixtureMaterialWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBorder> Border;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> WeaponName;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UImage> WeaponImage;

protected:
	FInventorySlot InventorySlot;

	bool bHasWeaponSlot = false;

// Texture
protected:
	UPROPERTY(EditAnywhere, Category = "Texture")
	TObjectPtr<UTexture2D> NormalTexture;

	UPROPERTY(EditAnywhere, Category = "Texture")
	TObjectPtr<UTexture2D> SelectedTexture;

public:
	UBSMixtureMaterialWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FORCEINLINE bool CheckHasWeaponSlot() const { return bHasWeaponSlot; }

public:
	void SetWeaponSlot(const FInventorySlot& InInventorySlot);
	void UnsetWeaponSlot();

protected:
	virtual void NativeConstruct() override;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	void RightClickForRemove();
};
