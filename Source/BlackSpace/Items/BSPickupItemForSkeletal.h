// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/BSInteractInterface.h"
#include "BSPickupItemForSkeletal.generated.h"

class ABSEquipmentBase;

UCLASS()
class BLACKSPACE_API ABSPickupItemForSkeletal 
	: public AActor
	, public IBSInteractInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<USkeletalMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<ABSEquipmentBase> EquipmentClass;

public:	
	ABSPickupItemForSkeletal();

	FORCEINLINE void SetEquipmentClass(const TSubclassOf<ABSEquipmentBase>& NewEquipmentClass) { EquipmentClass = NewEquipmentClass; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

	/* IBSInteractInterface Implement */
	virtual void Interact(AActor* Interactor) override;

};
