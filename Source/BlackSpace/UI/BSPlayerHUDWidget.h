// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSDefine.h"
#include "BSPlayerHUDWidget.generated.h"

class UBSStatBarWidget;
class UBSPlayerStatusWeaponWidget;
struct FInventorySlot;

UCLASS()
class BLACKSPACE_API UBSPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSStatBarWidget> HealthBarWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSStatBarWidget> StaminaBarWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSPlayerStatusWeaponWidget> MainWeaponWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSPlayerStatusWeaponWidget> SecondaryWeaponWidget;

public:
	UBSPlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;
	

public:
	void SetStatBarRatio(const EAttributeType& AttributeType, float InRatio);

protected:
	void SetMainWeaponState(const FInventorySlot& MainWeaponInfo);
	void SetSecondaryWeaponState(const FInventorySlot& SecondaryWeaponInfo);

};
