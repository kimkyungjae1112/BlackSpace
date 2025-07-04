// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BSDefine.h"
#include "BSAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnChangedAttribute, const EAttributeType&, float)
DECLARE_MULTICAST_DELEGATE(FOnDeath)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLACKSPACE_API UBSAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/* 체력, 스테미나 변경을 알리는 델리게이트 */
	FDelegateOnChangedAttribute OnChangedAttribute;

	/* 죽음을 알리는 델리게이트 */
	FOnDeath OnDeath;

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
	float RegenStaminaRate = 2.f;

	FTimerHandle StaminaRegenTimer;

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

public:
	bool CheckHasEnoughStamina(float StaminaCost) const;

	void ToggleStaminaRegen(bool bEnabled, float DelayTime = 2.f);

	void DecreaseStamina(float StaminaCost);

	void BroadcastAttributeChanged(const EAttributeType& InAttributeType) const;
		
	void TakeDamageAmount(float DamageAmount);

private:
	void RegenrateStaminaHandle();
};
