// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/BSSpawnEnemyForTest.h"
#include "Components/BoxComponent.h"
#include "Engine/TargetPoint.h"

#include "Characters/BSCharacterEnemy.h"

ABSSpawnEnemyForTest::ABSSpawnEnemyForTest()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);
}

void ABSSpawnEnemyForTest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSSpawnEnemyForTest::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
	{
		ABSCharacterEnemy* Enemy = GetWorld()->SpawnActor<ABSCharacterEnemy>(SpawnEnemyClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());
	}
}


