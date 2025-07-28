// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/BSEquipmentBase.h"
#include "GameFramework/Character.h"

ABSEquipmentBase::ABSEquipmentBase()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SetRootComponent(MeshComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

void ABSEquipmentBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSEquipmentBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(MeshComp))
	{
		StaticMeshComp->SetStaticMesh(MeshAsset);
	}
}

void ABSEquipmentBase::EquipItem()
{
}

void ABSEquipmentBase::UnequipItem()
{
}

void ABSEquipmentBase::OnceCalledSetWeaponDamage()
{
}

void ABSEquipmentBase::AttachToOwner(FName SocketName)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
		}
	}
}

void ABSEquipmentBase::DetachToOwner()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
