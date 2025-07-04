// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/BSWeaponBow.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Character.h"

ABSWeaponBow::ABSWeaponBow()
{
	BowMeshComp = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("Poseable Component"));
	BowMeshComp->SetupAttachment(RootComp);
}

void ABSWeaponBow::BeginPlay()
{
	Super::BeginPlay();

	CacheStringLocation = BowMeshComp->GetBoneLocationByName(StringBoneName, EBoneSpaces::ComponentSpace);
}

void ABSWeaponBow::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (SkeletalMeshAsset)
	{
		BowMeshComp->SetSkeletalMesh(SkeletalMeshAsset);
		
		SkeletalMeshComp->SetSkeletalMesh(nullptr);
		SkeletalMeshComp->SetVisibility(false);
		
		MeshComp->SetStaticMesh(nullptr);
		MeshComp->SetVisibility(false);
	}
}

void ABSWeaponBow::PullString(const FVector& HandSocketLoc)
{
	BowMeshComp->SetBoneLocationByName(StringBoneName, HandSocketLoc, EBoneSpaces::WorldSpace);
}

void ABSWeaponBow::ReleaseString()
{
	BowMeshComp->SetBoneLocationByName(StringBoneName, CacheStringLocation, EBoneSpaces::ComponentSpace);
}
