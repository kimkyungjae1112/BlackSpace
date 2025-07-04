// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSInventorySlot.h"
#include "BSEquipmentBase.generated.h"

UCLASS()
class BLACKSPACE_API ABSEquipmentBase : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Equipment | Mesh")
	TObjectPtr<UStaticMesh> MeshAsset;

	UPROPERTY(VisibleAnywhere, Category = "Equipment | Mesh")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, Category = "Equipment | Mesh")
	TObjectPtr<USkeletalMesh> SkeletalMeshAsset;

	UPROPERTY(VisibleAnywhere, Category = "Equipment | Mesh")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	TObjectPtr<USceneComponent> RootComp;

// Inventory Data
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Inventory")
	FInventorySlot InventoryInfo;

public:	
	ABSEquipmentBase();

	FORCEINLINE FInventorySlot GetInventoryInfo() const { return InventoryInfo; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void EquipItem();

	virtual void UnequipItem();

	virtual void AttachToOwner(FName SocketName);
};
