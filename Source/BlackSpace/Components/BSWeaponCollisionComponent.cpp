// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BSWeaponCollisionComponent.h"
#include "GenericTeamAgentInterface.h"

UBSWeaponCollisionComponent::UBSWeaponCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

void UBSWeaponCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UBSWeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}
}

void UBSWeaponCollisionComponent::TurnOnCollision()
{
	bIsCollisionEnabled = true;
}

void UBSWeaponCollisionComponent::TurnOffCollision()
{
	AlreadyHitActors.Empty();
	bIsCollisionEnabled = false;
}

void UBSWeaponCollisionComponent::SetWeaponMesh(UPrimitiveComponent* MeshComponent)
{
	WeaponMeshComp = MeshComponent;
}

void UBSWeaponCollisionComponent::AddIgnoreActor(AActor* Actor)
{
	IgnoredActors.Add(Actor);
}

void UBSWeaponCollisionComponent::RemoveIgnoreActor(AActor* Actor)
{
	IgnoredActors.Remove(Actor);
}

void UBSWeaponCollisionComponent::CollisionTrace()
{
	TArray<FHitResult> OutHits;

	const FVector Start = WeaponMeshComp->GetSocketLocation(TraceStartSocketName);
	const FVector End = WeaponMeshComp->GetSocketLocation(TraceEndSocketName);

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TraceRadius,
		TraceObjectTypes,
		false,
		IgnoredActors,
		DrawDebugType,
		OutHits,
		true
	);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();

			if (HitActor == nullptr || AlreadyHitActors.Contains(HitActor))
			{
				continue;
			}

			// 첫 번째 Owner = Weapon
			// 두 번째 Owner = Weapon 을 장착한 캐릭터
			ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(GetOwner()->GetOwner(), HitActor);

			if (Attitude != ETeamAttitude::Friendly)
			{
				AlreadyHitActors.Add(HitActor);

				if (OnHitActor.IsBound())
				{
					OnHitActor.Broadcast(Hit);
				}
			}
		}
	}
}

