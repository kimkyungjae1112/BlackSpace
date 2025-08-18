// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSMixtureMaterialWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "UI/BSInventoryMenuWidget.h"
#include "Components/BSInventoryComponent.h"
#include "BSInventorySlot.h"

FDelegateCanMixture UBSMixtureMaterialWidget::DelegateCanMixture;

UBSMixtureMaterialWidget::UBSMixtureMaterialWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSMixtureMaterialWidget::SetWeaponSlot(const FInventorySlot& InInventorySlot, int32 InIndex)
{
	Index = InIndex;
	bHasWeaponSlot = true;
	InventorySlot = InInventorySlot;

	if (InventorySlot.WeaponGrade == EWeaponGrade::Common)
	{
		UE_LOG(LogTemp, Display, TEXT("Weapon Grade : Common"));
	}
	else if (InventorySlot.WeaponGrade == EWeaponGrade::Rare)
	{
		UE_LOG(LogTemp, Display, TEXT("Weapon Grade : Rare"));
	}
	else if (InventorySlot.WeaponGrade == EWeaponGrade::Epic)
	{
		UE_LOG(LogTemp, Display, TEXT("Weapon Grade : Epic"));
	}
	WeaponName->SetText(InInventorySlot.Name);
	WeaponImage->SetBrushFromTexture(InInventorySlot.Image);
	WeaponImage->SetOpacity(1.f);
}

void UBSMixtureMaterialWidget::UnsetWeaponSlot()
{
	Index = -1;
	bHasWeaponSlot = false;
	InventorySlot = FInventorySlot();
	WeaponName->SetText(InventorySlot.Name);
	WeaponImage->SetBrushFromTexture(InventorySlot.Image);
	WeaponImage->SetOpacity(0.f);
}

void UBSMixtureMaterialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponImage->SetOpacity(0.f);
}

FReply UBSMixtureMaterialWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		RightClickForRemove();
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UBSMixtureMaterialWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

bool UBSMixtureMaterialWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool SuperResult = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return false;
}

void UBSMixtureMaterialWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (Border)
	{
		Border->SetBrushFromTexture(SelectedTexture);
	}
}

void UBSMixtureMaterialWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (Border)
	{
		Border->SetBrushFromTexture(NormalTexture);
	}
}

void UBSMixtureMaterialWidget::RightClickForRemove()
{
	if (InventorySlot.Quantity > 0)
	{
		if (APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
			{
				if (!bMixtured)
				{
					InventoryComp->AddToSlot(Index);

					UnsetWeaponSlot();

					if (DelegateCanMixture.IsBound())
					{
						DelegateCanMixture.ExecuteIfBound();
					}
				}
				else
				{
					InventoryComp->AddToSlot(InventorySlot);

					UnsetWeaponSlot();
				}

			}
		}
	}
}