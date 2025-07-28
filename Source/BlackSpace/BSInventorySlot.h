// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BSDefine.h"
#include "BSInventorySlot.generated.h"

class UTexture2D;
class ABSEquipmentBase;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Data")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Data")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Data")
	TObjectPtr<UTexture2D> Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Data")
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Data")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Data")
	EWeaponGrade WeaponGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Data")
	TSubclassOf<ABSEquipmentBase> ItemClass;
};

//USTRUCT(BlueprintType)
//struct FWeaponInfo : public FInventorySlot
//{
//	GENERATED_BODY()
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Data")
//	float AttackDamage;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Data")
//	float AttackSpeed;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Data")
//	float AttackRange;
//
//};
