// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSPlayerStatusWeaponWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Components/BSCombatComponent.h"

UBSPlayerStatusWeaponWidget::UBSPlayerStatusWeaponWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSPlayerStatusWeaponWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponImage->SetRenderOpacity(0.f);
}

void UBSPlayerStatusWeaponWidget::SetWeaponImage(UTexture2D* NewWeaponImage)
{
	WeaponImage->SetBrushFromTexture(NewWeaponImage);

	NewWeaponImage ? WeaponImage->SetRenderOpacity(1.f) : WeaponImage->SetRenderOpacity(0.f);
}

void UBSPlayerStatusWeaponWidget::SetWeaponName(const FText& NewWeaponName)
{
	WeaponName->SetText(NewWeaponName);
}

void UBSPlayerStatusWeaponWidget::SetCombatComp(UBSCombatComponent* NewCombatComp)
{
	CombatComp = NewCombatComp;
}

FReply UBSPlayerStatusWeaponWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		RightClickForUnequip();
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UBSPlayerStatusWeaponWidget::RightClickForUnequip()
{
	if (CurrentWeaponWidgetType == EWeaponWidgetType::MainWeapon)
	{
		CombatComp->SetUnequipMainWeapon();
	}
	else if (CurrentWeaponWidgetType == EWeaponWidgetType::SecondaryWeapon)
	{
		CombatComp->SetUnequipSecondaryWeapon();
	}
}
