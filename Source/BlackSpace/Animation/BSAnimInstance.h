// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/BSUpdateAnyTypeInterface.h"
#include "BSAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;

UCLASS()
class BLACKSPACE_API UBSAnimInstance 
	: public UAnimInstance
	, public IBSUpdateAnyTypeInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reference")
	TObjectPtr<ACharacter> Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reference")
	TObjectPtr<UCharacterMovementComponent> MovementComp;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool bIsIdle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool bIsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool bIsBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	float AimYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	float AimPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FVector Velocity;

/* Combat */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
	bool bCombatEnabled = true; // 임시

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
	EWeaponType WeaponType = EWeaponType::Uncombat;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
	void AnimNotify_ResetMovementInput();

	UFUNCTION()
	void AnimNotify_ResetState();

public:
	/* IBSUpdateAnyTypeInterface Implement */
	virtual void UpdateWeaponType(const EWeaponType& InWeaponType) override;
	virtual void UpdateBlcokingState(const bool bBlockingState) override;

protected:
	void OnChangeCombat(const bool InCombatEnabled);
};
