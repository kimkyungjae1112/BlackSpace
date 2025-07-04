// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BSPickupItem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "Equipments/BSEquipmentBase.h"
#include "Components/BSInventoryComponent.h"
#include "BSDefine.h"

ABSPickupItem::ABSPickupItem()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SetRootComponent(MeshComp);

	MeshComp->SetCollisionObjectType(COLLISION_OBJECT_INTERACTION);
	MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void ABSPickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSPickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (EquipmentClass)
	{
		if (ABSEquipmentBase* CDO = EquipmentClass->GetDefaultObject<ABSEquipmentBase>())
		{
			MeshComp->SetStaticMesh(CDO->MeshAsset);
			MeshComp->SetSimulatePhysics(true);
		}
	}
}

void ABSPickupItem::Interact(AActor* Interactor)
{
	if (EquipmentClass)
	{
		if (ABSEquipmentBase* CDO = EquipmentClass->GetDefaultObject<ABSEquipmentBase>())
		{
			if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			{
				if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
				{
					InventoryComp->AddToSlot(CDO->GetInventoryInfo());
				}
			}
		}
	}

	Destroy();
}


