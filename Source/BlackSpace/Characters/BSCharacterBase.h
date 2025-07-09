// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/BSCombatInterface.h"
#include "BSCharacterBase.generated.h"

class USoundCue;

UCLASS()
class BLACKSPACE_API ABSCharacterBase 
	: public ACharacter
	, public IBSCombatInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<USoundCue> ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<USoundCue> BlockingImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UParticleSystem> ImpactParticle;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UParticleSystem> BlockingImpactParticle;

public:
	ABSCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	/* IBSCombatInterface Implement */
	virtual void ActivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType) {};
	virtual void DeactivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType) {};

protected:
	virtual void OnDeath() {};
	virtual void ImpactEffect(const FVector& Location) {};
	virtual void HitReaction(const AActor* Attacker) {};
};
