// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/BSEquipmentBase.h"
#include "GameFramework/Character.h"

ABSEquipmentBase::ABSEquipmentBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComp->SetVisibility(false);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	SkeletalMeshComp->SetupAttachment(RootComp);
	SkeletalMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	SkeletalMeshComp->SetVisibility(false);
}

void ABSEquipmentBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSEquipmentBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshAsset)
	{
		MeshComp->SetStaticMesh(MeshAsset);
		MeshComp->SetVisibility(true);
		SkeletalMeshComp->SetVisibility(false);
	}
	else if (SkeletalMeshAsset)
	{
		SkeletalMeshComp->SetSkeletalMesh(SkeletalMeshAsset);
		SkeletalMeshComp->SetVisibility(true);
		MeshComp->SetVisibility(false);
	}
}

void ABSEquipmentBase::EquipItem()
{
}

void ABSEquipmentBase::UnequipItem()
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
