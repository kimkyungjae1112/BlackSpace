// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSPlayerHUDWidget.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

#include "UI/BSStatBarWidget.h"
#include "UI/BSPlayerStatusWeaponWidget.h"
#include "Components/BSAttributeComponent.h"
#include "Components/BSCombatComponent.h"
#include "BSInventorySlot.h"

UBSPlayerHUDWidget::UBSPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* Player = GetOwningPlayerPawn())
	{
		if (UBSAttributeComponent* AttributeComp = Player->GetComponentByClass<UBSAttributeComponent>())
		{
			AttributeComp->OnChangedAttribute.AddUObject(this, &ThisClass::SetStatBarRatio);
			AttributeComp->BroadcastAttributeChanged(EAttributeType::Stamina);
			AttributeComp->BroadcastAttributeChanged(EAttributeType::Health);
		}

		if (UBSCombatComponent* CombatComp = Player->GetComponentByClass<UBSCombatComponent>())
		{
			CombatComp->OnChangedMainWeapon.AddUObject(this, &ThisClass::SetMainWeaponState);
			CombatComp->OnChangedSecondaryWeapon.AddUObject(this, &ThisClass::SetSecondaryWeaponState);
		}
	}

	if (HitImage)
	{
		HitImage->SetOpacity(0.f);
	}
}

void UBSPlayerHUDWidget::SetStatBarRatio(const EAttributeType& AttributeType, float InRatio)
{
	switch (AttributeType)
	{
	case EAttributeType::Stamina:
		StaminaBarWidget->SetStatBarRatio(InRatio);
		break;
	case EAttributeType::Health:
		HealthBarWidget->SetStatBarRatio(InRatio);
		break;
	}
}

void UBSPlayerHUDWidget::ShowHitEffect()
{
	if (FadeOut_Anim)
	{
		PlayAnimation(FadeOut_Anim);
	}
}

void UBSPlayerHUDWidget::SetMainWeaponState(const FInventorySlot& MainWeaponInfo)
{
	MainWeaponWidget->SetWeaponImage(MainWeaponInfo.Image);
	MainWeaponWidget->SetWeaponName(MainWeaponInfo.Name);
}

void UBSPlayerHUDWidget::SetSecondaryWeaponState(const FInventorySlot& SecondaryWeaponInfo)
{
	SecondaryWeaponWidget->SetWeaponImage(SecondaryWeaponInfo.Image);
	SecondaryWeaponWidget->SetWeaponName(SecondaryWeaponInfo.Name);
}
