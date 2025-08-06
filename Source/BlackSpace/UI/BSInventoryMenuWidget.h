// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSInventoryMenuWidget.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UImage;
class UBSMixtureWeaponWidget;
struct FInventorySlot;

UCLASS()
class BLACKSPACE_API UBSInventoryMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	static int32 CurrentTabIndex;

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSMixtureWeaponWidget> WBPMixtureWeapon;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> CurrentTabText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> NextTabText;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> PreviousTabText;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UImage> WidgetIndicator0;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UImage> WidgetIndicator1;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UImage> WidgetIndicator2;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UWidgetSwitcher> TabSwitcher;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FText> TabNameMappingMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UImage*> WidgetIndicator;


public:
	UBSInventoryMenuWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void SetWeaponSlot(const FInventorySlot& InInventorySlot, int32 InIndex) const;

	void OnLeftTab();
	void OnRightTab();

protected:
	virtual void NativeConstruct() override;

protected:
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	void UpdateTab();
};
