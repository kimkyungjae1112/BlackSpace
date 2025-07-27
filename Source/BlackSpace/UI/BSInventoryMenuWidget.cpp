// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSInventoryMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"

#include "UI/BSMixtureWeaponWidget.h"
#include "BSInventorySlot.h"

int32 UBSInventoryMenuWidget::CurrentTabIndex = 1;

UBSInventoryMenuWidget::UBSInventoryMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSInventoryMenuWidget::SetWeaponSlot(const FInventorySlot& InInventorySlot) const
{
	WBPMixtureWeapon->SetMixtureWeaponSlot(InInventorySlot);
}

void UBSInventoryMenuWidget::OnLeftTab()
{
	if (TabSwitcher == nullptr)
	{
		return;
	}

	(CurrentTabIndex <= 0) ? CurrentTabIndex = 2 : CurrentTabIndex--;

	UpdateTab();
}

void UBSInventoryMenuWidget::OnRightTab()
{
	if (TabSwitcher == nullptr)
	{
		return;
	}

	CurrentTabIndex = (CurrentTabIndex + 1) % 3;

	UpdateTab();
}

void UBSInventoryMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TabNameMappingMap.Add(0, FText::FromString(TEXT("플레이어 상태")));
	TabNameMappingMap.Add(1, FText::FromString(TEXT("인벤토리")));
	TabNameMappingMap.Add(2, FText::FromString(TEXT("무기 조합")));

	if (CurrentTabIndex >= 0 && TabNameMappingMap.Num() > CurrentTabIndex)
	{
		CurrentTabText->SetText(TabNameMappingMap[CurrentTabIndex]);
	}

	WidgetIndicator.Add(WidgetIndicator0);
	WidgetIndicator.Add(WidgetIndicator1);
	WidgetIndicator.Add(WidgetIndicator2);

	for (int32 i = 0; i < WidgetIndicator.Num(); ++i)
	{
		if (i == CurrentTabIndex)
		{
			WidgetIndicator[i]->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			continue;
		}
		WidgetIndicator[i]->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.2f));
	}
}

void UBSInventoryMenuWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

bool UBSInventoryMenuWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool SuperResult = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return false;
}

void UBSInventoryMenuWidget::UpdateTab()
{
	if (TabSwitcher)
	{
		TabSwitcher->SetActiveWidgetIndex(CurrentTabIndex);
	}

	if (CurrentTabIndex >= 0 && TabNameMappingMap.Num() > CurrentTabIndex)
	{
		CurrentTabText->SetText(TabNameMappingMap[CurrentTabIndex]);
	}

	if (NextTabText && PreviousTabText)
	{
		int32 TempIndex = CurrentTabIndex;
		const int32 RightTabIndex = (TempIndex + 1) % 3;
		const int32 LeftTabIndex = (TempIndex <= 0) ? TempIndex = 2 : TempIndex--;

		NextTabText->SetText(TabNameMappingMap[RightTabIndex]);
		PreviousTabText->SetText(TabNameMappingMap[LeftTabIndex]);
	}

	for (int32 i = 0; i < WidgetIndicator.Num(); ++i)
	{
		if (i == CurrentTabIndex)
		{
			WidgetIndicator[i]->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			continue;
		}
		WidgetIndicator[i]->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.2f));
	}
}

