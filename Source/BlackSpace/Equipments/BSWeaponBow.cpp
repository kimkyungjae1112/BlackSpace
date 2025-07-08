// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/BSWeaponBow.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Character.h"

#include "Components/BSWeaponCollisionComponent.h"

ABSWeaponBow::ABSWeaponBow()
{
	if (BowMeshComp = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("Poseable Component")))
	{
		MeshComp->DestroyComponent();
		MeshComp = BowMeshComp;
		RootComponent = MeshComp;
	}
}

void ABSWeaponBow::BeginPlay()
{
	Super::BeginPlay();

	CacheStringLocation = BowMeshComp->GetBoneLocationByName(StringBoneName, EBoneSpaces::ComponentSpace);
}

void ABSWeaponBow::EquipItem()
{
	Super::EquipItem();

	if (ACharacter* Player = Cast<ACharacter>(GetOwner()))
	{
		CollisionComp->SetWeaponMesh(Player->GetMesh());
		SecondaryCollisionComp->SetWeaponMesh(Player->GetMesh());
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
