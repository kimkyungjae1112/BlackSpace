// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BSSpawnNiagara.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraFunctionLibrary.h"

void UAnimNotify_BSSpawnNiagara::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !Template)
	{
		return;
	}

	UWorld* World = MeshComp->GetWorld();
	if (!World)
	{
		return;
	}

	if (MeshComp->DoesSocketExist(SocketName) || SocketName.IsNone())
	{
		const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);

		const FVector SpawnLocation = SocketTransform.TransformPosition(LocationOffset);
		const FRotator SpawnRotation = (SocketTransform.GetRotation() * FQuat(RotationOffset)).Rotator();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			World,				
			Template,			
			SpawnLocation,		
			SpawnRotation,		
			Scale,				
			true,				
			true,				
			ENCPoolMethod::None 
		);
	}
}
