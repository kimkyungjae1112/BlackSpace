// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BSDefine.h"
#include "BSAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnChangedAttribute, const EAttributeType&, float)
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE(FOnMaxPosture)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLACKSPACE_API UBSAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/* 체력, 스테미나 변경을 알리는 델리게이트 */
	FDelegateOnChangedAttribute OnChangedAttribute;

	/* 죽음을 알리는 델리게이트 */
	FOnDeath OnDeath;

	/* 체간 게이지가 모두 찬 것을 알리는 델리게이트 */
	FOnMaxPosture OnMaxPosture;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float BaseStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float BaseHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxPosture = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float BasePosture = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float WeightPosture = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float RegenStaminaRate = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float RegenPostureRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Defense = 1.f;

	FTimerHandle StaminaRegenTimer;
	FTimerHandle PostureRegenTimer;


public:	
	UBSAttributeComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE float GetBaseStamina() const { return BaseStamina; }
	FORCEINLINE float GetStaminaRatio() const { return BaseStamina / MaxStamina; }

	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetBaseHealth() const { return BaseHealth; }
	FORCEINLINE float GetHealthRatio() const { return BaseHealth / MaxHealth; }

	FORCEINLINE float GetMaxPosture() const { return MaxPosture; }
	FORCEINLINE float GetBasePosture() const { return BasePosture; }
	FORCEINLINE float GetPostureRatio() const { return BasePosture / MaxPosture; }

	FORCEINLINE int32 GetDefense() const { return Defense; }

public:
	bool CheckHasEnoughStamina(float StaminaCost) const;

	void ToggleStaminaRegen(bool bEnabled, float DelayTime = 1.5f);

	void TogglePostureRegen(bool bEnabled, float DelayTime = 1.5f);

	void DecreaseStamina(float StaminaCost);

	void BroadcastAttributeChanged(const EAttributeType& InAttributeType) const;
		
	void TakeDamageAmount(float DamageAmount);

	void TakePostureAmount(float DamageAmount);

private:
	void RegenerateStaminaHandle();
	void RegeneratePostureHandle();
};
