// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSBossHealthBarWidget.h"
#include "Components/TextBlock.h"

#include "Components/BSAttributeComponent.h"
#include "UI/BSStatBarWidget.h"

void UBSBossHealthBarWidget::Init(UBSAttributeComponent* AttributeComp, FText ThisName)
{
	if (AttributeComp)
	{
		AttributeComp->OnChangedAttribute.AddUObject(this, &ThisClass::OnAttributeChanged);
		AttributeComp->BroadcastAttributeChanged(EAttributeType::Health);
	}

	if (NameWidget)
	{
		NameWidget->SetText(ThisName);
	}
}

void UBSBossHealthBarWidget::OnAttributeChanged(const EAttributeType& AttributeType, float Ratio)
{
	switch (AttributeType)
	{
	case EAttributeType::Health:
		HealthBarWidget->SetStatBarRatio(Ratio);
		break;
	}
}
