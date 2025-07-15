// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BSCharacterEnemyMaldrith.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

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

	if (MaldrithMusicAsset)
	{
		if (!bStartedBossMusic)
		{
			bStartedBossMusic = true;
			MaldrithMusicComp = UGameplayStatics::SpawnSound2D(this, MaldrithMusicAsset);
			MaldrithMusicComp->FadeIn(1.f);
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

	if (BackAttackWidgetComp)
	{
		BackAttackWidgetComp->DestroyComponent();
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

	if (IsValid(MaldrithMusicComp))
	{
		MaldrithMusicComp->FadeOut(2.f, 0);
	}
}
