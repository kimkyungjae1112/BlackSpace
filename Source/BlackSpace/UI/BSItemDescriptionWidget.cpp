// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSItemDescriptionWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

#include "BSInventorySlot.h"

UBSItemDescriptionWidget::UBSItemDescriptionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSItemDescriptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EntireBox->SetVisibility(ESlateVisibility::Hidden);
}

void UBSItemDescriptionWidget::SetDescription(const FInventorySlot& InDescriptionSlot)
{
	if (InDescriptionSlot.Quantity > 0)
	{
		EntireBox->SetVisibility(ESlateVisibility::Visible);
		GradeText->SetText(GetWeaponGradeText(InDescriptionSlot.WeaponGrade));
		GradeText->SetColorAndOpacity(GetWeaponGradeTextColor(InDescriptionSlot.WeaponGrade));
		DamageText->SetText(FText::AsNumber(InDescriptionSlot.Damage));
		TypeText->SetText(InDescriptionSlot.Name);
	}
}

void UBSItemDescriptionWidget::UnsetDescription()
{
	EntireBox->SetVisibility(ESlateVisibility::Hidden);
}

FText UBSItemDescriptionWidget::GetWeaponGradeText(const EWeaponGrade& Grade) const
{
	switch (Grade)
	{
	case EWeaponGrade::Common: return FText::FromString(TEXT("Common"));
	case EWeaponGrade::Rare: return FText::FromString(TEXT("Rare"));
	case EWeaponGrade::Epic: return FText::FromString(TEXT("Epic"));
	case EWeaponGrade::Legendary: return FText::FromString(TEXT("Legendary"));
	default: return FText::FromString(TEXT("Unknown"));
	}
}

FLinearColor UBSItemDescriptionWidget::GetWeaponGradeTextColor(const EWeaponGrade& Grade) const
{
	switch (Grade)
	{
	case EWeaponGrade::Common: return FLinearColor(0.6f, 0.6f, 0.58f, 1.f);
	case EWeaponGrade::Rare: return FLinearColor(0.4f, 0.65f, 0.6f, 1.f);
	case EWeaponGrade::Epic: return FLinearColor(0.65f, 0.45f, 0.85f, 1.f);
	case EWeaponGrade::Legendary: return FLinearColor(0.95f, 0.75f, 0.3f, 1.f);
	default: return FLinearColor(1.f, 1.f, 1.f, 1.f);
	}
}
