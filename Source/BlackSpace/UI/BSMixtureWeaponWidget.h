// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSMixtureWeaponWidget.generated.h"

class UBSMixtureMaterialWidget;
class UButton;
struct FInventorySlot;

UCLASS()
class BLACKSPACE_API UBSMixtureWeaponWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSMixtureMaterialWidget> WBPMixtureFirstWeapon;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSMixtureMaterialWidget> WBPMixtureSecondWeapon;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBSMixtureMaterialWidget> WBPMixturedWeapon;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> MixtureButton;

public:
	UBSMixtureWeaponWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

public:
	void SetMixtureWeaponSlot(const FInventorySlot& InInventorySlot) const;

private:
	void DisableMixtureButton() const;

	UFUNCTION()
	void ClickMixtureButton();
};
