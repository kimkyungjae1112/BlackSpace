// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/BSCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "BSDefine.h"
#include "BSCharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FDelegateOnAttackStart)

class USoundCue;
class UBSAudioManagerSubsystem;

UCLASS()
class BLACKSPACE_API ABSCharacterBase 
	: public ACharacter
	, public IBSCombatInterface
	, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	/* 플레이어의 공격 시작을 알리는 델리게이트 */
	static FDelegateOnAttackStart OnAttackStart;

protected:
	UPROPERTY(EditAnywhere, Category = "Team")
	FGenericTeamId TeamId = FGenericTeamId(0);

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

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void OnDeath() {};
	virtual void ImpactEffect(const FVector& Location) {};
	virtual void HitReaction(const AActor* Attacker, const EDamageType& DamageType) {};

protected:
	UBSAudioManagerSubsystem* GetAudioManager();
};
