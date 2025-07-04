// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BSCharacterBase.h"
#include "BSCharacterEnemy.generated.h"

class UBSStateComponent;
class UBSAttributeComponent;
class USoundCue;

UCLASS()
class BLACKSPACE_API ABSCharacterEnemy : public ABSCharacterBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UBSStateComponent> StateComp;

	UPROPERTY(VisibleAnywhere, Category = "Enemy | Component")
	TObjectPtr<UBSAttributeComponent> AttributeComp;

protected:
	UPROPERTY(EditAnywhere, Category = "Enemy | Montage")
	TObjectPtr<UAnimMontage> HitReactAnimFront;

	UPROPERTY(EditAnywhere, Category = "Enemy | Montage")
	TObjectPtr<UAnimMontage> HitReactAnimBack;

	UPROPERTY(EditAnywhere, Category = "Enemy | Montage")
	TObjectPtr<UAnimMontage> HitReactAnimLeft;

	UPROPERTY(EditAnywhere, Category = "Enemy | Montage")
	TObjectPtr<UAnimMontage> HitReactAnimRight;

protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<USoundCue> ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UParticleSystem> ImpactParticle;

public:
	ABSCharacterEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void OnDeath();

protected:
	void ImpactEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker);
	UAnimMontage* GetHitReactAnimation(const AActor* Attacker) const;
};
