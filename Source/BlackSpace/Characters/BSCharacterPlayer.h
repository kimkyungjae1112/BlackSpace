// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BSCharacterBase.h"
#include "GameplayTagContainer.h"
#include "Interface/BSComboWindowInterface.h"
#include "Interface/BSBowFireInterface.h"
#include "Interface/BSPlayerAttackedInterface.h"
#include "Interface/BSPlayerHUDInterface.h"
#include "Interface/BSDialogueEndInterface.h"
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
class UBSRotationComponent;
class UMotionWarpingComponent;
class UBSTargetingComponent;
class UBSPlayerStatusWidget;
class USoundCue;
class UBSPauseMenuWidget;
class ABSPlayerController;
class ABSArrow;

USTRUCT()
struct FMoveSpeed
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Stat")
	TArray<float> MoveSpeeds;
};

UCLASS()
class BLACKSPACE_API ABSCharacterPlayer
	: public ABSCharacterBase
	, public IBSComboWindowInterface
	, public IBSBowFireInterface
	, public IBSPlayerAttackedInterface
	, public IBSPlayerHUDInterface
	, public IBSDialogueEndInterface
{
	GENERATED_BODY()

// Input
protected:
	// Dialogue Input Mapping Context 를 어떻게 넣을지 고민,,,,,
	// 1. EWeaponType에 그냥 Dialogue 타입 추가 -> 근데 애니메이션 빵구
	// 2. Dialogue 일 때만 예외처리 하기 -> 이걸로 결정
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UBSRotationComponent> RotationComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UMotionWarpingComponent> MotionWarpComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UBSTargetingComponent> TargetingComp;

// UI
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UBSPlayerHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UBSPlayerHUDWidget> HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UBSPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UBSPauseMenuWidget> PauseMenuWidget;

// Sound
protected:
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> BlockingImpactSound;

// Particle
protected:
	UPROPERTY(EditAnywhere, Category = "Particle")
	TObjectPtr<UParticleSystem> ImpactParticle;

	UPROPERTY(EditAnywhere, Category = "Particle")
	TObjectPtr<UParticleSystem> BlockingImpactParticle;

// Stat
protected:
	UPROPERTY(EditAnywhere, Category = "Stat")
	TMap<EWeaponType, FMoveSpeed> MoveSpeedMap;

	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bIsSprinting = false;

	// Fire 공격, Sprint 플래그
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bAiming = false;

	// Camera View, Animation 플래그
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bProgressAiming = false;

	// 적과 마주보고 있는지 검사하는 플래그
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bFacingEnemy = false;

	// 현재 패링할 수 있는 상태인지 검사하는 플래그
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bParryEnabled = false;

	// 상대방을 백어택 할 수 있는지 검사하는 플래그
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bBackAttack = false;

	// 상대방 체간 급소 공격을 할 수 있는지 검사하는 플래그
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bPostureAttack = false;

	// 구르기 할 때 무적 프레임 플래그
	UPROPERTY(EditAnywhere, Category = "Stat")
	bool bEnabledIFrame = false;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	EWeaponType CurrentWeaponType;

	
// Combo
protected:
	bool bComboSequenceRunning = false;
	bool bCanComboInput = false;
	bool bSavedComboInput = false;
	int32 ComboCounter = 0;
	FTimerHandle ComboResetTimerHandle;

// Parry
protected:
	FTimerHandle ParryStartTimer;
	FTimerHandle ParryEndTimer;

	// Arrow
protected:
	UPROPERTY(EditAnywhere, Category = "Arrow")
	TSubclassOf<ABSArrow> ArrowClass;

	UPROPERTY(VisibleAnywhere, Category = "Arrow")
	TObjectPtr<ABSArrow> Arrow;

	// BackAttack
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APawn> VitalAttackTarget;

	// Dialogue
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> DialogueTarget;

public:
	ABSCharacterPlayer();

	FORCEINLINE APawn* GetVitalAttackTarget() const { return VitalAttackTarget; }
	FORCEINLINE bool IsEnabledIFrame() const { return bEnabledIFrame; }

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
	virtual void ToggleIFrame(const bool bEnableIFrame) override;

	/* IBSBowFireInterface Implement */
	virtual void BowFireFinished() override;

	/* IBSCombatInterface Implement */
	virtual void ActivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType) override;

	/* IBSPlayerAttackedInterface Implement */
	virtual void GSwordSpecialAttackExecuted(UAnimMontage* Montage) override;
	void GSwordSpecialAttackExecutedMotionWarp() const;

	/* IBSPlayerHUDInterface Implement */
	virtual UBSPlayerHUDWidget* GetHUDWidget() const override;

	/* IBSDialogueEndInterface Implement */
	virtual void EndDialogue() override;
	virtual void OptionDialogue() override;
	virtual void EndOptionDialogue() override;

	void AttackFinished(const float ComboResetDelay);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	bool IsDead() const;

	// 피격
protected:
	virtual void OnDeath() override;
	virtual void ImpactEffect(const FVector& Location) override;
	void BlockImpactEffect(const FVector& Location);
	virtual void HitReaction(const AActor* Attacker, const EDamageType& DamageType) override;

private:
	bool IsMoving() const;
	bool CanRolling() const;
	bool CanChangeWeapon() const;
	bool CanBlockingStance() const;
	bool CanAttackBlocking() const;
	bool CanParrying() const;
	bool CanDetectForBackAttack() const;
	bool CanDetectForDialogue() const;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint();
	void StopSprint();
	void Rolling();
	void ToggleInventoryMenu();
	void TogglePauseMenu();
	void InventoryLeftPage();
	void InventoryRightPage();
	void Interaction();
	void ChangeWeapon();
	void Dialogue();
	void NextDialogue();
	void SkipDialogue();
	void ForTestMaxHealth();

	// 공격
	void LightAttack();
	void HeavyAttack();
	void SpecialAttack();

	// 방어
	void Blocking();
	void BlockingEnd();
	void Parry();
	void ParryEnd();

	// 락온
	void LockOnTarget();
	void LeftTarget();
	void RightTarget();

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
	void PullString();

	void PullStringStart();
	void PullStringCancel();
	void PullStringComplete();

	void FireArrow(const FGameplayTag& AttackType);
	FVector CalculateViewDirection() const;

	void ToggleCameraViewAdjust();
	void ToggleAimingFlag(bool InIsAiming);

	// 급소 공격
private:
	bool DetectForBackAttackTarget(OUT FHitResult& OutResult);
	void BackAttackMotionWarp();
	void PostureAttackMotionWarp();

	// 대화
private:
	bool DetectForDialogue(OUT FHitResult& OutResult);

// Input 변경
private:
	// 현재 무기 타입도 바꿈
	void ChagnedWeapon(const struct FInventorySlot&);
	void SetInputMapping(const EWeaponType& InWeaponType);
	void SetInputMapping(class UInputMappingContext* InInputMappingContext);

private:
	ABSPlayerController* GetPlayerController() const;
};
