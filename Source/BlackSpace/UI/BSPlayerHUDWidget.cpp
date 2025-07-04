// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSPlayerHUDWidget.h"
#include "UI/BSStatBarWidget.h"
#include "Components/BSAttributeComponent.h"

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
		}
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
		break;
	default:
		break;
	}
}
