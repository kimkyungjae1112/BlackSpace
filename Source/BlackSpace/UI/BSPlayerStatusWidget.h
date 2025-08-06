// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSPlayerStatusWidget.generated.h"

class UTextBlock;
class UBSPlayerStatusWeaponWidget;
struct FInventorySlot;

UCLASS()
class BLACKSPACE_API UBSPlayerStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> DamageTextNum;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> AttackSpeedText;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> AttackRangeText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSPlayerStatusWeaponWidget> MainWeaponWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSPlayerStatusWeaponWidget> SecondaryWeaponWidget;

public:
	UBSPlayerStatusWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

public:
	void SetMainWeaponState(const FInventorySlot& MainWeaponInfo);
	void SetSecondaryWeaponState(const FInventorySlot& SecondaryWeaponInfo);


};
