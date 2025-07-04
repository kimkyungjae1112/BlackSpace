// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BSCharacterBase.generated.h"

UCLASS()
class BLACKSPACE_API ABSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ABSCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


};
