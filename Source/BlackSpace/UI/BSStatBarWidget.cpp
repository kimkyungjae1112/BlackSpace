// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BSStatBarWidget.h"
#include "Components/ProgressBar.h"

UBSStatBarWidget::UBSStatBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBSStatBarWidget::SetStatBarRatio(float InRatio)
{
	StaminaBar->SetPercent(InRatio);
}
