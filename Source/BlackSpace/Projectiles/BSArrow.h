// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BSArrow.generated.h"

class UProjectileMovementComponent;

UCLASS()
class BLACKSPACE_API ABSArrow : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Arrow | Socket")
	FName ArrowHeadSocket;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Arrow | Collision Value")
	float TraceRadius = 3.f;

	UPROPERTY(EditAnywhere, Category = "Arrow | Collision Value")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditAnywhere, Category = "Arrow | Collision Value")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

	UPROPERTY(EditAnywhere, Category = "Arrow | Collision Value")
	bool bHit = false;

	TArray<AActor*> IgnoredActors;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Arrow | Component")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Arrow | Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Arrow | Component")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComp;

public:	
	ABSArrow();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
public:
	void AddIgnoreActor(AActor* Actor);
	void RemoveIgnoreActor(AActor* Actor);

public:
	void Fire(const FVector& InDirection);

protected:
	void CollisionTrace();
};
