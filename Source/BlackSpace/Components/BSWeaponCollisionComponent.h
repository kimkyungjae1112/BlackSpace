// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BSWeaponCollisionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitActor, const FHitResult&)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), meta=(DisplayName = "Weapon Collision"))
class BLACKSPACE_API UBSWeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHitActor OnHitActor;

protected:
	UPROPERTY(EditAnywhere, Category = "Collision | Socket")
	FName TraceStartSocketName;

	UPROPERTY(EditAnywhere, Category = "Collision | Socket")
	FName TraceEndSocketName;

protected:
	UPROPERTY(EditAnywhere, Category = "Collision | Set Value")
	float TraceRadius = 20.f;

	UPROPERTY(EditAnywhere, Category = "Collision | Set Value")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditAnywhere, Category = "Collision | Set Value")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

	TArray<AActor*> IgnoredActors;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPrimitiveComponent> WeaponMeshComp;

	UPROPERTY(VisibleAnywhere)
	TSet<AActor*> AlreadyHitActors;

	bool bIsCollisionEnabled = false;

public:	
	UBSWeaponCollisionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void TurnOnCollision();

	void TurnOffCollision();

	void SetWeaponMesh(UPrimitiveComponent* MeshComponent);

	void AddIgnoreActor(AActor* Actor);

	void RemoveIgnoreActor(AActor* Actor);

protected:
	void CollisionTrace();
};
