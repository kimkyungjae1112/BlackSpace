// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BSRotationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLACKSPACE_API UBSRotationComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(VisibleAnywhere)
	bool bShouldRotation = false;

public:	
	UBSRotationComponent();

	FORCEINLINE void SetTargetActor(AActor* InTargetActor) { TargetActor = InTargetActor; }
	FORCEINLINE void ToggleRotation(bool bRotation) { bShouldRotation = bRotation; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
