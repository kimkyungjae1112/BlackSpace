// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BSInputData.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class BLACKSPACE_API UBSInputData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common | IMC")
	TObjectPtr<UInputMappingContext> IMC_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common | Move")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common | Look")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common | Sprint")
	TObjectPtr<UInputAction> IA_SprintAndRolling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common | Interact")
	TObjectPtr<UInputAction> IA_Interact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common | UI")
	TObjectPtr<UInputAction> IA_ToggleInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common | UI")
	TObjectPtr<UInputAction> IA_TogglePlayerStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common | Weapon")
	TObjectPtr<UInputAction> IA_ChangeWeapon;

	/***********************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword | Attack")
	TObjectPtr<UInputAction> IA_SwordAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword | Attack")
	TObjectPtr<UInputAction> IA_SwordHeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword | Block")
	TObjectPtr<UInputAction> IA_Blocking;
	
	/***********************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow | Attack")
	TObjectPtr<UInputAction> IA_BowAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow | Attack")
	TObjectPtr<UInputAction> IA_BowHeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow | Attack")
	TObjectPtr<UInputAction> IA_BowStringPull;

};
