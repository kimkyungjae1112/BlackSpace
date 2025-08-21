// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/BSBossStageDoor.h"

ABSBossStageDoor::ABSBossStageDoor()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	LeftDoorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door Component"));
	LeftDoorComp->SetupAttachment(RootComp);

	RightDoorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door Component"));
	RightDoorComp->SetupAttachment(RootComp);
}

void ABSBossStageDoor::OpenDoor()
{
	LeftDoorComp->SetWorldRotation(LeftDoorComp->GetComponentRotation() - FRotator(0.f, 0.f, 100.f));
	RightDoorComp->SetWorldRotation(RightDoorComp->GetComponentRotation() + FRotator(0.f, 0.f, 100.f));
}

