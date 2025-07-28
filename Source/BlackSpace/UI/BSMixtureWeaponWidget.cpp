// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSMixtureWeaponWidget.h"
#include "Components/Button.h"

#include "UI/BSMixtureMaterialWidget.h"

UBSMixtureWeaponWidget::UBSMixtureWeaponWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UBSMixtureMaterialWidget::DelegateCanMixture.BindUObject(this, &ThisClass::DisableMixtureButton);
}

void UBSMixtureWeaponWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MixtureButton)
	{
		MixtureButton->OnClicked.AddDynamic(this, &ThisClass::ClickMixtureButton);
		MixtureButton->SetIsEnabled(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Mixture Button!"));
	}
}

void UBSMixtureWeaponWidget::SetMixtureWeaponSlot(const FInventorySlot& InInventorySlot) const
{
	if (!WBPMixtureFirstWeapon->CheckHasWeaponSlot())
	{
		WBPMixtureFirstWeapon->SetWeaponSlot(InInventorySlot);

		if (WBPMixtureSecondWeapon->CheckHasWeaponSlot())
		{
			MixtureButton->SetIsEnabled(true);
		}
	}
	else
	{
		WBPMixtureSecondWeapon->SetWeaponSlot(InInventorySlot);

		MixtureButton->SetIsEnabled(true);
	}
}

void UBSMixtureWeaponWidget::DisableMixtureButton() const
{
	MixtureButton->SetIsEnabled(false);
}

void UBSMixtureWeaponWidget::ClickMixtureButton()
{
	FInventorySlot FirstSlot = WBPMixtureFirstWeapon->GetInventorySlot();
	FInventorySlot SecondSlot = WBPMixtureSecondWeapon->GetInventorySlot();
	FInventorySlot NewWeapon = FInventorySlot();

	// 둘 중 높은 등급 기준으로 혼합
	EWeaponGrade BaseGrade = EWeaponGrade::Common;
	if (FirstSlot.WeaponGrade >= SecondSlot.WeaponGrade)
	{
		NewWeapon = FirstSlot;
		BaseGrade = FirstSlot.WeaponGrade;
	}
	else
	{
		NewWeapon = SecondSlot;
		BaseGrade = SecondSlot.WeaponGrade;
	}

	EWeaponGrade ResultGrade = BaseGrade;

	const float RandomValue = FMath::FRand();

	switch (BaseGrade)
	{
	case EWeaponGrade::Common:
		if (RandomValue < 1.f) ResultGrade = EWeaponGrade::Rare;
		break;
	case EWeaponGrade::Rare:
		if (RandomValue < 1.f) ResultGrade = EWeaponGrade::Epic;
		break;
	case EWeaponGrade::Epic:
		if (RandomValue < 1.f) ResultGrade = EWeaponGrade::Legendary;
		break;
	case EWeaponGrade::Legendary:
		// 최상급 등급: 변화 없음
		break;
	default:
		break;
	}
	
	NewWeapon.WeaponGrade = ResultGrade;

	WBPMixturedWeapon->SetWeaponSlot(NewWeapon);

	WBPMixtureFirstWeapon->UnsetWeaponSlot();
	WBPMixtureSecondWeapon->UnsetWeaponSlot();

	MixtureButton->SetIsEnabled(false);
}
