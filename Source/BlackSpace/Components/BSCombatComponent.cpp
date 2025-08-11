// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BSCombatComponent.h"

#include "BSInventorySlot.h"
#include "Equipments/BSWeapon.h"
#include "Components/BSInventoryComponent.h"

UBSCombatComponent::UBSCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UBSCombatComponent::BeginPlay()
{
	Super::BeginPlay();


}


void UBSCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBSCombatComponent::SetWeapon(ABSWeapon* NewWeapon)
{
	if (MainWeapon != nullptr)
	{
		SetSecondaryWeapon(NewWeapon);
		return;
	}

	bHasMainWeapon = true;

	bCombatEnabled = true;

	MainWeapon = NewWeapon;
	MainWeapon->OnUpdateWeaponType();

	if (GetOwner()->ActorHasTag(TEXT("Player")))
	{
		OnChangedMainWeapon.Broadcast(MainWeapon->GetInventoryInfo());
	}
}

void UBSCombatComponent::SetSecondaryWeapon(ABSWeapon* NewWeapon)
{
	if (SecondaryWeapon)
	{
		// 메인, 보조 무기 모두 가지고 있을 때 장비 착용 요청
		ChangeMainWeapon(NewWeapon);
		return;
	}

	bHasSecondaryWeapon = true;

	SecondaryWeapon = NewWeapon;
	SecondaryWeapon->SetActorHiddenInGame(true);

	if (GetOwner()->ActorHasTag(TEXT("Player")))
	{
		OnChangedSecondaryWeapon.Broadcast(SecondaryWeapon->GetInventoryInfo());
	}
}

void UBSCombatComponent::ChangeWeapon()
{
	if (CanChangeWeapon())
	{
		MainWeapon->SetActorHiddenInGame(true);
		SecondaryWeapon->SetActorHiddenInGame(false);

		ABSWeapon* TempWeapon = MainWeapon;

		MainWeapon = SecondaryWeapon;
		MainWeapon->AttachToOwner(MainWeapon->GetEquipSocket());
		MainWeapon->OnUpdateWeaponType();
		if (GetOwner()->ActorHasTag(TEXT("Player")))
		{
			OnChangedMainWeapon.Broadcast(MainWeapon->GetInventoryInfo());
		}

		SecondaryWeapon = TempWeapon;
		SecondaryWeapon->DetachToOwner();
		if (GetOwner()->ActorHasTag(TEXT("Player")))
		{
			OnChangedSecondaryWeapon.Broadcast(SecondaryWeapon->GetInventoryInfo());
		}
	}
}

void UBSCombatComponent::SetUnequipMainWeapon()
{
	if (UBSInventoryComponent* InventoryComp = GetOwner()->GetComponentByClass<UBSInventoryComponent>())
	{
		InventoryComp->AddToSlot(MainWeapon);
	}

	MainWeapon->UnequipItem();
	MainWeapon = nullptr;

	bHasMainWeapon = false;

	bCombatEnabled = false;

	if (GetOwner()->ActorHasTag(TEXT("Player")))
	{
		OnChangedMainWeapon.Broadcast(FInventorySlot());
	}
}

void UBSCombatComponent::SetUnequipMainWeapon(const bool bCanDestroy)
{
	if (UBSInventoryComponent* InventoryComp = GetOwner()->GetComponentByClass<UBSInventoryComponent>())
	{
		InventoryComp->AddToSlot(MainWeapon);
	}

	MainWeapon->UnequipItem();
	if (bCanDestroy)
	{
		MainWeapon->Destroy();
	}
	MainWeapon = nullptr;

	bHasMainWeapon = false;

	bCombatEnabled = false;

	if (GetOwner()->ActorHasTag(TEXT("Player")))
	{
		OnChangedMainWeapon.Broadcast(FInventorySlot());
	}
}

void UBSCombatComponent::SetUnequipSecondaryWeapon()
{
	if (UBSInventoryComponent* InventoryComp = GetOwner()->GetComponentByClass<UBSInventoryComponent>())
	{
		InventoryComp->AddToSlot(SecondaryWeapon);
	}

	SecondaryWeapon->DetachToOwner();
	SecondaryWeapon = nullptr;

	bHasSecondaryWeapon = false;

	if (GetOwner()->ActorHasTag(TEXT("Player")))
	{
		OnChangedSecondaryWeapon.Broadcast(FInventorySlot());
	}
}

void UBSCombatComponent::ChangeMainWeapon(ABSWeapon* NewWeapon)
{
	MainWeapon->DetachToOwner();

	MainWeapon = NewWeapon;
	MainWeapon->OnUpdateWeaponType();

	if (GetOwner()->ActorHasTag(TEXT("Player")))
	{
		OnChangedMainWeapon.Broadcast(MainWeapon->GetInventoryInfo());
	}
}