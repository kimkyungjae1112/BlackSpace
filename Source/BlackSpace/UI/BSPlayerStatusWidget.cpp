// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSPlayerStatusWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/TextBlock.h"

#include "Components/BSCombatComponent.h"
#include "UI/BSPlayerStatusWeaponWidget.h"
#include "BSInventorySlot.h"

UBSPlayerStatusWidget::UBSPlayerStatusWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSPlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (UBSCombatComponent* CombatComp = Player->GetComponentByClass<UBSCombatComponent>())
		{
			CombatComp->OnChangedMainWeapon.AddUObject(this, &UBSPlayerStatusWidget::SetMainWeaponState);
			CombatComp->OnChangedSecondaryWeapon.AddUObject(this, &UBSPlayerStatusWidget::SetSecondaryWeaponState);

			MainWeaponWidget->SetCombatComp(CombatComp);
			SecondaryWeaponWidget->SetCombatComp(CombatComp);
		}
	}
}

void UBSPlayerStatusWidget::SetMainWeaponState(const FInventorySlot& MainWeaponInfo)
{
	MainWeaponWidget->SetWeaponImage(MainWeaponInfo.Image);
	MainWeaponWidget->SetWeaponName(MainWeaponInfo.Name);
}

void UBSPlayerStatusWidget::SetSecondaryWeaponState(const FInventorySlot& SecondaryWeaponInfo)
{
	SecondaryWeaponWidget->SetWeaponImage(SecondaryWeaponInfo.Image);
	SecondaryWeaponWidget->SetWeaponName(SecondaryWeaponInfo.Name);
}

