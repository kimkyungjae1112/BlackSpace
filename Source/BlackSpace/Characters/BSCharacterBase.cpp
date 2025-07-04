// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BSCharacterBase.h"


ABSCharacterBase::ABSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

void ABSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}




