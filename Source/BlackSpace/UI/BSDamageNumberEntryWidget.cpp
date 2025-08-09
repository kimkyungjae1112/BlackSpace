// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSDamageNumberEntryWidget.h"
#include "Components/TextBlock.h"

void UBSDamageNumberEntryWidget::SetText(const FText& InText)
{
	if (DamageText) { DamageText->SetText(InText); }
}

void UBSDamageNumberEntryWidget::SetAlpha(float InAlpha)
{
    CurrentAlpha = FMath::Clamp(InAlpha, 0.f, 1.f);
    if (DamageText)
    {
        FSlateColor SlateCol = DamageText->ColorAndOpacity;
        FLinearColor C = SlateCol.GetSpecifiedColor();
        C.A = CurrentAlpha;
        DamageText->SetColorAndOpacity(C);
    }
}

void UBSDamageNumberEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
