// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSKnockBack.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GenericTeamAgentInterface.h"

void UAnimNotify_BSKnockBack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const FVector OriginLocation = MeshComp->GetSocketLocation(LocationSocketName);

	if (bDrawDebug)
	{
		UKismetSystemLibrary::DrawDebugSphere(MeshComp, OriginLocation, DamageRadius, 12, FLinearColor::White, 2.f, 1.f);
	}

	APawn* OwnerPawn = Cast<APawn>(MeshComp->GetOwner());
	if (!OwnerPawn)
	{
		return;
	}

	AController* OwnerController = OwnerPawn->GetController();
	if (!OwnerController)
	{
		return;
	}

	TArray<FHitResult> HitResults;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerPawn);

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		OwnerPawn->GetWorld(),
		OriginLocation,
		OriginLocation,
		DamageRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (bHit)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(OwnerPawn, HitActor);

			if (Attitude != ETeamAttitude::Friendly)
			{
				TArray<AActor*> IgnoreActors;
				IgnoreActors.Add(OwnerPawn);

				UGameplayStatics::ApplyRadialDamage(OwnerPawn, BaseDamage, OriginLocation, DamageRadius, nullptr, IgnoreActors, nullptr, OwnerController, false, ECC_Visibility);
			}
		}
	}
}