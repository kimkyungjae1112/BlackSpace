// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BSPlayerStatusWeaponWidget.generated.h"

class UTextBlock;
class UBorder;
class UTexture2D;

class UBSCombatComponent;

UENUM(BlueprintType)
enum class EWeaponWidgetType : uint8
{
	MainWeapon,
	SecondaryWeapon
};

UCLASS()
class BLACKSPACE_API UBSPlayerStatusWeaponWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> WeaponName;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UBorder> WeaponImage;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBSCombatComponent> CombatComp;

protected:
	UPROPERTY(EditAnywhere)
	EWeaponWidgetType CurrentWeaponWidgetType;

public:
	UBSPlayerStatusWeaponWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

public:
	void SetWeaponImage(UTexture2D* NewWeaponImage);
	void SetWeaponName(const FText& NewWeaponName);

public:
	void SetCombatComp(UBSCombatComponent* NewCombatComp);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	void RightClickForUnequip();
};
