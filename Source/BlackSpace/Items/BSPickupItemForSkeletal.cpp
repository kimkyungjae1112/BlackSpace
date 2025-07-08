// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BSPickupItemForSkeletal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "Equipments/BSEquipmentBase.h"
#include "Components/BSInventoryComponent.h"
#include "Components/BSCombatComponent.h"
#include "BSDefine.h"

ABSPickupItemForSkeletal::ABSPickupItemForSkeletal()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Component"));
	SetRootComponent(MeshComp);

	MeshComp->SetCollisionObjectType(COLLISION_OBJECT_INTERACTION);
	MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void ABSPickupItemForSkeletal::BeginPlay()
{
	Super::BeginPlay();

}

void ABSPickupItemForSkeletal::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (EquipmentClass)
	{
		if (ABSEquipmentBase* CDO = EquipmentClass->GetDefaultObject<ABSEquipmentBase>())
		{
			MeshComp->SetSkeletalMesh(CDO->SkeletalMeshAsset);
			MeshComp->SetSimulatePhysics(true);
		}
	}
}

void ABSPickupItemForSkeletal::Interact(AActor* Interactor)
{
	if (EquipmentClass)
	{
		if (ABSEquipmentBase* CDO = EquipmentClass->GetDefaultObject<ABSEquipmentBase>())
		{
			if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			{
				if (UBSCombatComponent* CombatComp = Player->GetComponentByClass<UBSCombatComponent>())
				{
					if (CombatComp->GetMainWeapon() == nullptr || CombatComp->GetSecondaryWeapon() == nullptr)
					{
						FActorSpawnParameters Param;
						Param.Owner = Player;

						ABSEquipmentBase* Weapon = GetWorld()->SpawnActor<ABSEquipmentBase>(EquipmentClass, Player->GetActorTransform(), Param);
						if (Weapon)
						{
							Weapon->EquipItem();
						}
					}
					else
					{
						if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
						{
							InventoryComp->AddToSlot(CDO->GetInventoryInfo());
						}
					}
				}
			}
		}
	}

	Destroy();
}

