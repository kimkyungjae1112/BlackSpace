// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BSCharacterBase.h"
#include "GameplayTagContainer.h"
#include "BSDefine.h"
#include "Interface/BSComboWindowInterface.h"
#include "Interface/BSBowFireInterface.h"
#include "BSCharacterPlayer.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
struct FInputActionValue;

class UBSInputData;
class UBSPlayerHUDWidget;
class UBSAttributeComponent;
class UBSInventoryComponent;
class UBSCombatComponent;
class UBSStateComponent;
class UBSPlayerStatusWidget;
class ABSPlayerController;

UCLASS()
class BLACKSPACE_API ABSCharacterPlayer
	: public ABSCharacterBase
	, public IBSComboWindowInterface
	, public IBSBowFireInterface
{
	GENERATED_BODY()

// Input
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TMap<EWeaponType, UInputMappingContext*> InputMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UBSInputData> InputData;

// Camera
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float BaseSpringArmLength = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float AimingSpringArmLength = 200.f;

// Component
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UBSAttributeComponent> AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UBSInventoryComponent> InventoryComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UBSCombatComponent> CombatComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UBSStateComponent> StateComp;

// UI
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UBSPlayerHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UBSPlayerHUDWidget> HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UBSPlayerStatusWidget> PlayerStatusWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UBSPlayerStatusWidget> PlayerStatusWidget;

// Stat
protected:
	UPROPERTY(EditAnywhere, Category = "Stat")
	float WalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float SprintSpeed = 850.f;

	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bIsSprinting = false;

	// Fire 공격, Sprint 플래그
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bAiming = false;

	// Camera View, Animation 플래그
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bProgressAiming = false;

// Combo
protected:
	bool bComboSequenceRunning = false;
	bool bCanComboInput = false;
	bool bSavedComboInput = false;
	int32 ComboCounter = 0;
	FTimerHandle ComboResetTimerHandle;

public:
	ABSCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/* IBSComboWindowInterface Implement */
	virtual void EnableComboWindow() override;
	virtual void DisableComboWindow() override;

	/* IBSBowFireInterface Implement */
	virtual void BowFireFinished() override;

	void AttackFinished(const float ComboResetDelay);

private:
	bool IsMoving() const;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint();
	void StopSprint();
	void ToggleInventory();
	void TogglePlayerStatus();
	void Interaction();

	// 공격
	void LightAttack();
	void HeavyAttack();
	void SpecialAttack();

// 콤보 공격
private:
	/* 현재 상태에서 수행 가능한 일반 공격 */
	FGameplayTag GetAttackPerform() const;

	bool CanPerformAttack(const FGameplayTag& AttackType);

	void ExecuteComboAttack(const FGameplayTag& AttackType);

	void DoAttack(const FGameplayTag& AttackType);

	void ResetCombo();

// 활 공격
private:
	bool CanPullingString();

	void PullStringStart();
	void PullString();
	void PullStringCancel();
	void PullStringComplete();

	void FireArrow(const FGameplayTag& AttackType);

	void ToggleCameraViewAdjust();
	void ToggleAimingFlag(bool InIsAiming);

// Input 변경
private:
	void ChagnedWeapon(const struct FInventorySlot&);
	void SetInputMapping(const EWeaponType& InWeaponType);

private:
	ABSPlayerController* GetPlayerController() const;

};
