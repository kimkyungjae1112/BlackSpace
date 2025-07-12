// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BSCharacterEnemyMaldrith.h"
#include "Components/WidgetComponent.h"

#include "UI/BSBossHealthBarWidget.h"

ABSCharacterEnemyMaldrith::ABSCharacterEnemyMaldrith()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABSCharacterEnemyMaldrith::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ABSCharacterEnemyMaldrith::SeesTarget(AActor* InTargetActor)
{
	if (IsValid(InTargetActor))
	{
		if (BossHealthBarWidget)
		{
			BossHealthBarWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void ABSCharacterEnemyMaldrith::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidgetComp)
	{
		HealthBarWidgetComp->DestroyComponent();
	}

	if (BossHealthBarWidgetClass)
	{
		BossHealthBarWidget = CreateWidget<UBSBossHealthBarWidget>(GetWorld(), BossHealthBarWidgetClass);
		if (BossHealthBarWidget)
		{
			BossHealthBarWidget->Init(AttributeComp, BossName);
			BossHealthBarWidget->AddToViewport();
			BossHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABSCharacterEnemyMaldrith::OnDeath()
{
	Super::OnDeath();

	if (BossHealthBarWidget)
	{
		BossHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
