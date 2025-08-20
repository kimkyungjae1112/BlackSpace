// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSMixtureWeaponWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

#include "UI/BSMixtureMaterialWidget.h"
#include "Components/BSInventoryComponent.h"

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
}

void UBSMixtureWeaponWidget::SetMixtureWeaponSlot(const FInventorySlot& InInventorySlot, int32 Index)
{
	if (!WBPMixtureFirstWeapon->CheckHasWeaponSlot())
	{
		WBPMixtureFirstWeapon->SetWeaponSlot(InInventorySlot, Index);
		FirstWeaponIndex = Index;

		if (WBPMixtureSecondWeapon->CheckHasWeaponSlot())
		{
			MixtureButton->SetIsEnabled(true);
		}
	}
	else
	{
		WBPMixtureSecondWeapon->SetWeaponSlot(InInventorySlot, Index);

		MixtureButton->SetIsEnabled(true);
	}
}

void UBSMixtureWeaponWidget::DisableMixtureButton() const
{
	MixtureButton->SetIsEnabled(false);
}

void UBSMixtureWeaponWidget::ClickMixtureButton()
{
	APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(Player == nullptr)
	{
		return;
	}

	UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>();
	if (InventoryComp == nullptr)
	{
		return;
	}

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

	const int32 RandomValue = FMath::RandRange(1, 100);

	switch (BaseGrade)
	{
	case EWeaponGrade::Common:
		if (RandomValue <= 100)
		{
			ResultGrade = EWeaponGrade::Rare;
		}
		break;
	case EWeaponGrade::Rare:
		if (RandomValue <= 120)
		{
			ResultGrade = EWeaponGrade::Epic;
		}
		break;
	case EWeaponGrade::Epic:
		if (RandomValue <= 110)
		{
			ResultGrade = EWeaponGrade::Legendary;
		}
		break;
	case EWeaponGrade::Legendary:
		break;
	default:
		break;
	}
	
	NewWeapon.WeaponGrade = ResultGrade;

	WBPMixturedWeapon->SetWeaponSlot(NewWeapon, FirstWeaponIndex);

	InventoryComp->RemoveWeapon(WBPMixtureFirstWeapon->GetHasIndex());
	InventoryComp->RemoveWeapon(WBPMixtureSecondWeapon->GetHasIndex());

	WBPMixtureFirstWeapon->UnsetWeaponSlot();
	WBPMixtureSecondWeapon->UnsetWeaponSlot();

	MixtureButton->SetIsEnabled(false);
}
