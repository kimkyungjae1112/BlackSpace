// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSPlayerExecuted.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Interface/BSPlayerAttackedInterface.h"

UAnimNotify_BSPlayerExecuted::UAnimNotify_BSPlayerExecuted(const FObjectInitializer& ObjectInitializer)
{
}

void UAnimNotify_BSPlayerExecuted::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* OwnerCharacter = MeshComp->GetOwner())
	{
		if (ACharacter* Character = Cast<ACharacter>(OwnerCharacter))
		{
			const FVector Start = MeshComp->GetSocketLocation(SocketName);
			const FVector End = Start;

			FHitResult HitResult;

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(MeshComp->GetOwner());

			bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
				Character,
				Start,
				End,
				Radius,
				ObjectTypes,
				false,
				ActorsToIgnore,
				DrawDebugType,
				HitResult,
				true
			);

			if (bHit)
			{
				if (UAnimInstance* Anim = Character->GetMesh()->GetAnimInstance())
				{
					UAnimMontage* CurrentExecuteMontage = Anim->GetCurrentActiveMontage();
					if (CurrentExecuteMontage)
					{
						Anim->Montage_JumpToSection(TEXT("Attack"), CurrentExecuteMontage);
					}
				}

				if (IBSPlayerAttackedInterface* Interface = Cast<IBSPlayerAttackedInterface>(HitResult.GetActor()))
				{
					UWorld* World = Character->GetWorld();

					AActor* Target = HitResult.GetActor();
					check(Target);

					const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Target->GetActorLocation(), Character->GetActorLocation());
					Target->SetActorRotation(Rotation);
					
					const FVector Location = Character->GetMesh()->GetSocketLocation(TEXT("GreateSwordSpecialAttack"));
					Target->SetActorLocation(Location);

					Interface->GSwordSpecialAttackExecuted(ExecutedMontage);
				}
			}
		}
	}
}
