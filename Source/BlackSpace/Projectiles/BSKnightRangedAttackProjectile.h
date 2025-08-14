// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSKnightRangedAttackProjectile.generated.h"

class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class BLACKSPACE_API ABSKnightRangedAttackProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UNiagaraComponent> NiagaraComp;
		
protected:
	UPROPERTY()
	FVector Direction;

	UPROPERTY(EditAnywhere)
	float LifeTime = 5.f;

	UPROPERTY(EditAnywhere)
	float Damage = 50.f;

public:	
	ABSKnightRangedAttackProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetDirection(const FVector& InDirection);

private:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
