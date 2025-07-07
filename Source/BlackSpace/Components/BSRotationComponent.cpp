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

	if (bShouldRotation == false) return;

	if (bOwnerIsAI && TargetActor)
	{
		const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
		GetOwner()->SetActorRotation(FRotator(0.f, Rotation.Yaw, 0.f));
	}
}

void UBSRotationComponent::SetRotationPlayer(const FVector& DesireDirection)
{
	if (bOwnerIsPlayer)
	{
		// 어떻게 부드럽게 회전시킬까

		/*const FRotator CurrentRotation = GetOwner()->GetActorRotation();
		const FRotator FinalRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 10.f);*/

		const FRotator TargetRotation = DesireDirection.Rotation();

		GetOwner()->SetActorRotation(FRotator(0.f, TargetRotation.Yaw, 0.f));
	}
}

