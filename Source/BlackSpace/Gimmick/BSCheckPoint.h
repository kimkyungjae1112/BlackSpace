// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSCheckPoint.generated.h"

class UBoxComponent;

UCLASS()
class BLACKSPACE_API ABSCheckPoint : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> PosComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CheckPointUIClass;

public:	
	ABSCheckPoint();

	FORCEINLINE USceneComponent* GetPosComp() const { return PosComp; }

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
