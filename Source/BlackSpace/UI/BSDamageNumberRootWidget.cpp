// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSDamageNumberRootWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

#include "UI/BSDamageNumberEntryWidget.h"

void UBSDamageNumberRootWidget::InitPool(int32 InitialSize)
{
	if (!Canvas || !EntryClass) 
	{ 
		return; 
	}

	Pool.Reset();

	for (int32 i = 0; i < InitialSize; ++i)
	{
		AddOne();
	}
}

int32 UBSDamageNumberRootWidget::BorrowEntry(const FText& DamageText)
{
	int32 Idx = FindFreeIndex();
	if (Idx == INDEX_NONE)
	{
		Idx = AddOne();

		if (Idx == INDEX_NONE) 
		{ 
			return INDEX_NONE; 
		}
	}

	FEntrySlot& EntrySlot = Pool[Idx];
	EntrySlot.InUse = true;
	EntrySlot.Widget->SetText(DamageText);
	EntrySlot.Widget->SetAlpha(1.f);
	EntrySlot.Widget->SetRenderScale(FVector2D(1.f, 1.f));
	EntrySlot.Widget->SetVisibility(ESlateVisibility::HitTestInvisible);
	return Idx;
}

void UBSDamageNumberRootWidget::ReturnEntry(int32 Index)
{
	if (!Pool.IsValidIndex(Index)) 
	{ 
		return; 
	}

	FEntrySlot& EntrySlot = Pool[Index];
	if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(EntrySlot.Widget->Slot))
	{
		CanvasPanelSlot->SetPosition(FVector2D(-10000.f, -10000.f));
	}

	EntrySlot.Widget->SetVisibility(ESlateVisibility::Collapsed);
	EntrySlot.InUse = false;
}

void UBSDamageNumberRootWidget::UpdateEntry(int32 Index, const FVector2D& ScreenPos, float Alpha, float Scale, bool bVisible)
{
	if (!Pool.IsValidIndex(Index)) 
	{ 
		return; 
	}

	FEntrySlot& EntrySlot = Pool[Index];
	if (!EntrySlot.InUse) 
	{ 
		return; 
	}

	if (auto* PanelSlot = Cast<UCanvasPanelSlot>(EntrySlot.Widget->Slot))
	{
		PanelSlot->SetPosition(ScreenPos);
	}

	EntrySlot.Widget->SetAlpha(Alpha);
	EntrySlot.Widget->SetRenderScale(FVector2D(Scale, Scale));
	EntrySlot.Widget->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UBSDamageNumberRootWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

int32 UBSDamageNumberRootWidget::AddOne()
{
	if (!Canvas || !EntryClass) 
	{ 
		return INDEX_NONE; 
	}

	UBSDamageNumberEntryWidget* EntryWidget = CreateWidget<UBSDamageNumberEntryWidget>(GetOwningPlayer(), EntryClass);
	if (!EntryWidget) 
	{ 
		return INDEX_NONE; 
	}

	Canvas->AddChild(EntryWidget);
	if (auto* PanelSlot = Cast<UCanvasPanelSlot>(EntryWidget->Slot))
	{
		PanelSlot->SetAutoSize(true);
		PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		PanelSlot->SetPosition(FVector2D(-10000.f, -10000.f)); // 화면 밖 대기
	}
	EntryWidget->SetVisibility(ESlateVisibility::Collapsed);

	FEntrySlot EntrySlot;
	EntrySlot.Widget = EntryWidget;
	EntrySlot.InUse = false;

	return Pool.Add(EntrySlot);
}

int32 UBSDamageNumberRootWidget::FindFreeIndex() const
{
	for (int32 i = 0; i < Pool.Num(); ++i)
	{
		if (!Pool[i].InUse) 
		{ 
			return i; 
		}
	}

	return INDEX_NONE;
}
