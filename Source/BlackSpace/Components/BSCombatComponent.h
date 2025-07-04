// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "BSCombatComponent.generated.h"

class ABSWeapon;
struct FInventorySlot;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedMainWeapon, const FInventorySlot&)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedSecondaryWeapon, const FInventorySlot&)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLACKSPACE_API UBSCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/* 메인 무기가 바뀌었을 때 호출되는 델리게이트 */
	FOnChangedMainWeapon OnChangedMainWeapon;

	/* 보조 무기가 바뀌었을 때 호출되는 델리게이트 */
	FOnChangedSecondaryWeapon OnChangedSecondaryWeapon;

protected:
	UPROPERTY()
	TObjectPtr<ABSWeapon> MainWeapon;

	UPROPERTY()
	TObjectPtr<ABSWeapon> SecondaryWeapon;

	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;

	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;

protected:
	bool bHasMainWeapon = false;
	bool bHasSecondaryWeapon = false;

public:	
	UBSCombatComponent();

	FORCEINLINE ABSWeapon* GetMainWeapon() const { return MainWeapon; }
	FORCEINLINE ABSWeapon* GetSecondaryWeapon() const { return SecondaryWeapon; }

	FORCEINLINE bool CheckHasMainWeapon() const { return bHasMainWeapon; }
	FORCEINLINE bool CheckHasSecondaryWeapon() const { return bHasSecondaryWeapon; }

	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; }
	FORCEINLINE void SetCombatEnabled(const bool NewEnabled) { bCombatEnabled = NewEnabled; }

	FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; }
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackType) { LastAttackType = NewAttackType; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetWeapon(ABSWeapon* NewWeapon);
	void SetSecondaryWeapon(ABSWeapon* NewWeapon);

	void SetUnequipMainWeapon();
	void SetUnequipSecondaryWeapon();

private:
	// 메인, 보조 무기 모두 가지고 있을 때 장비 착용 요청할 때 호출
	void ChangeMainWeapon(ABSWeapon* NewWeapon);
};
