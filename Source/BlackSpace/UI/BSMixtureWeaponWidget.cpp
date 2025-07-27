// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSMixtureWeaponWidget.h"

#include "UI/BSMixtureMaterialWidget.h"

UBSMixtureWeaponWidget::UBSMixtureWeaponWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSMixtureWeaponWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBSMixtureWeaponWidget::SetMixtureWeaponSlot(const FInventorySlot& InInventorySlot) const
{
	if (!WBPMixtureFirstWeapon->CheckHasWeaponSlot())
	{
		WBPMixtureFirstWeapon->SetWeaponSlot(InInventorySlot);
	}
	else
	{
		WBPMixtureSecondWeapon->SetWeaponSlot(InInventorySlot);
	}
}
