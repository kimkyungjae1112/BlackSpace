// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BSRotationComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBSRotationComponent::UBSRotationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBSRotationComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UBSRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TargetActor == nullptr) return;
	if (bShouldRotation == false) return;

	const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
	GetOwner()->SetActorRotation(FRotator(0.f, Rotation.Yaw, 0.f));
}

