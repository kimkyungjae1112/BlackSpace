// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/BSCheckPoint.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"

#include "GameModes/BSGameMode.h"

ABSCheckPoint::ABSCheckPoint()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);

	PosComp = CreateDefaultSubobject<USceneComponent>(TEXT("Pos Component"));
	PosComp->SetupAttachment(BoxComp);
}

void ABSCheckPoint::BeginPlay()
{
	Super::BeginPlay();

}

void ABSCheckPoint::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor->ActorHasTag(TEXT("Player")))
	{
		UUserWidget* CheckpointUI = CreateWidget<UUserWidget>(GetWorld(), CheckPointUIClass);
		if (CheckpointUI)
		{
			CheckpointUI->AddToViewport();
		}

		if (ABSGameMode* GameMode = Cast<ABSGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->SetCheckpoint(this);
		}

		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FTimerHandle FinishTimer;
		GetWorld()->GetTimerManager().SetTimer(FinishTimer, [this, CheckpointUI]()
			{
				CheckpointUI->RemoveFromParent();
			}, 3.f, false);
	}
}


