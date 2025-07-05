// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/BSWeapon.h"
#include "BSDefine.h"
#include "Interface/BSBowInterface.h"
#include "BSWeaponBow.generated.h"

class UPoseableMeshComponent;
class UBSWeaponCollisionComponent;

UCLASS()
class BLACKSPACE_API ABSWeaponBow 
	: public ABSWeapon
	, public IBSBowInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Bow")
	FName StringBoneName = TEXT("bow_string");

	UPROPERTY(VisibleAnywhere, Category = "Bow")
	FVector CacheStringLocation = FVector::ZeroVector;

// Component
protected:
	UPROPERTY(VisibleAnywhere, Category = "Equipment | Component")
	TObjectPtr<UPoseableMeshComponent> BowMeshComp;

public:
	ABSWeaponBow();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void EquipItem() override;

public:
	virtual void PullString(const FVector& HandSocketLoc) override;
	virtual void ReleaseString() override;
};
