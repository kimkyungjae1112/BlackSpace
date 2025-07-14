// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BSCharacterPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "Engine/DamageEvents.h"
#include "MotionWarpingComponent.h"

#include "Data/BSInputData.h"
#include "BSGameplayTag.h"
#include "Equipments/BSWeaponBow.h"
#include "Components/BSAttributeComponent.h"
#include "Components/BSInventoryComponent.h"
#include "Components/BSCombatComponent.h"
#include "Components/BSStateComponent.h"
#include "Components/BSRotationComponent.h"
#include "UI/BSPlayerHUDWidget.h"
#include "UI/BSPlayerStatusWidget.h"
#include "Interface/BSInteractInterface.h"
#include "Interface/BSBowInterface.h"
#include "Interface/BSUpdateAnyTypeInterface.h"
#include "Interface/BSAIControllerInterface.h"
#include "Interface/BSEnemyInterface.h"
#include "Player/BSPlayerController.h"
#include "Animation/BSAnimInstance.h"
#include "Projectiles/BSArrow.h"

ABSCharacterPlayer::ABSCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = BaseSpringArmLength;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	AttributeComp = CreateDefaultSubobject<UBSAttributeComponent>(TEXT("Attribute Component"));
	AttributeComp->OnDeath.AddUObject(this, &ThisClass::OnDeath);

	CombatComp = CreateDefaultSubobject<UBSCombatComponent>(TEXT("Combat Component"));
	CombatComp->OnChangedMainWeapon.AddUObject(this, &ThisClass::ChagnedWeapon);

	StateComp = CreateDefaultSubobject<UBSStateComponent>(TEXT("State Component"));

	InventoryComp = CreateDefaultSubobject<UBSInventoryComponent>(TEXT("Inventory Component"));

	RotationComp = CreateDefaultSubobject<UBSRotationComponent>(TEXT("Rotation Component"));
	RotationComp->bOwnerIsPlayer = true;

	MotionWarpComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping Component"));

	Tags.Add(TEXT("Player"));
}

void ABSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UBSPlayerHUDWidget>(GetWorld(), HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}

	PlayerStatusWidget = CreateWidget<UBSPlayerStatusWidget>(GetWorld(), PlayerStatusWidgetClass);
	if (PlayerStatusWidget)
	{
		PlayerStatusWidget->AddToViewport();
		PlayerStatusWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABSCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanDetectForBackAttack())
	{
		FHitResult HitResult;
		bool bHit = DetectForBackAttackTarget(HitResult);

		if (bHit)
		{
			VitalAttackTarget = Cast<APawn>(HitResult.GetActor());

			if (VitalAttackTarget)
			{
				if (IBSAIControllerInterface* AIInterface = Cast<IBSAIControllerInterface>(VitalAttackTarget->GetController()))
				{
					if (AIInterface->IsDetectedPlayer() == false)
					{
						if (IBSEnemyInterface* EnemyInterface = Cast<IBSEnemyInterface>(VitalAttackTarget))
						{
							if (bBackAttack != bHit)
							{
								bBackAttack = bHit;
								EnemyInterface->ToggleBackAttackWidgetVisibility(bHit);
							}
						}
					}

					if (IBSEnemyInterface* EnemyInterface = Cast<IBSEnemyInterface>(VitalAttackTarget))
					{
						bPostureAttack = EnemyInterface->IsEnabledPostureAttack();
					}
				}
			}
		}
		else
		{
			if (IBSEnemyInterface* EnemyInterface = Cast<IBSEnemyInterface>(VitalAttackTarget))
			{
				bBackAttack = false;
				bPostureAttack = false;
				EnemyInterface->ToggleBackAttackWidgetVisibility(false);
			}
			VitalAttackTarget = nullptr;
		}
	}
}

void ABSCharacterPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputData->IMC_Default, 0);
		}
	}
}

void ABSCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputData->IA_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(InputData->IA_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(InputData->IA_SprintAndRolling, ETriggerEvent::Triggered, this, &ThisClass::Sprint);
		EnhancedInputComponent->BindAction(InputData->IA_SprintAndRolling, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
		EnhancedInputComponent->BindAction(InputData->IA_SprintAndRolling, ETriggerEvent::Canceled, this, &ThisClass::Rolling);
		EnhancedInputComponent->BindAction(InputData->IA_ToggleInventory, ETriggerEvent::Started, this, &ThisClass::ToggleInventory);
		EnhancedInputComponent->BindAction(InputData->IA_TogglePlayerStatus, ETriggerEvent::Started, this, &ThisClass::TogglePlayerStatus);
		EnhancedInputComponent->BindAction(InputData->IA_Interact, ETriggerEvent::Started, this, &ThisClass::Interaction);
		EnhancedInputComponent->BindAction(InputData->IA_ChangeWeapon, ETriggerEvent::Started, this, &ThisClass::ChangeWeapon);

		/* Sword & Poleram */
		EnhancedInputComponent->BindAction(InputData->IA_SwordAttack, ETriggerEvent::Canceled, this, &ThisClass::LightAttack);
		EnhancedInputComponent->BindAction(InputData->IA_SwordAttack, ETriggerEvent::Triggered, this, &ThisClass::SpecialAttack);
		EnhancedInputComponent->BindAction(InputData->IA_SwordHeavyAttack, ETriggerEvent::Started, this, &ThisClass::HeavyAttack);
		EnhancedInputComponent->BindAction(InputData->IA_Blocking, ETriggerEvent::Started, this, &ThisClass::Blocking);
		EnhancedInputComponent->BindAction(InputData->IA_Blocking, ETriggerEvent::Completed, this, &ThisClass::BlockingEnd);

		/* Bow */
		EnhancedInputComponent->BindAction(InputData->IA_BowStringPull, ETriggerEvent::Started, this, &ThisClass::PullStringStart);
		EnhancedInputComponent->BindAction(InputData->IA_BowStringPull, ETriggerEvent::Canceled, this, &ThisClass::PullStringCancel);
		EnhancedInputComponent->BindAction(InputData->IA_BowStringPull, ETriggerEvent::Triggered, this, &ThisClass::PullStringComplete);
	}
}

void ABSCharacterPlayer::EnableComboWindow()
{
	bCanComboInput = true;
}

void ABSCharacterPlayer::DisableComboWindow()
{
	check(CombatComp);

	bCanComboInput = false;

	if (bSavedComboInput)
	{
		bSavedComboInput = false;
		ComboCounter++;
		DoAttack(CombatComp->GetLastAttackType());
	}
}

void ABSCharacterPlayer::BowFireFinished()
{
	check(StateComp);

	StateComp->ToggleMovementInput(true);

	bAiming = false;
	ToggleAimingFlag(false);
	ToggleCameraViewAdjust();
}

void ABSCharacterPlayer::ActivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType)
{
	check(CombatComp);

	if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
	{
		Weapon->ActivateWeaponCollision(WeaponCollisionType);
	}
}

void ABSCharacterPlayer::DeactivateWeaponCollision(const EWeaponCollisionType& WeaponCollisionType)
{
	check(CombatComp);

	if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
	{
		Weapon->DeactivateWeaponCollision(WeaponCollisionType);
	}
}

void ABSCharacterPlayer::AttackFinished(const float ComboResetDelay)
{
	check(StateComp);

	StateComp->ToggleMovementInput(true);

	// ComboResetDelay 후에 콤보 시퀀스 완전 종료
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ThisClass::ResetCombo, ComboResetDelay, false);
}

float ABSCharacterPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	check(AttributeComp);
	check(StateComp);

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 적과 대치중인 방향인지?
	bFacingEnemy = UKismetMathLibrary::InRange_FloatFloat(GetDotProductTo(EventInstigator->GetPawn()), -0.1f, 1.f);

	if (CanParrying())
	{
		if (IBSCombatInterface* CombatInterface = Cast<IBSCombatInterface>(EventInstigator->GetPawn()))
		{
			AttributeComp->ToggleStaminaRegen(false);
			AttributeComp->DecreaseStamina(20.f);
			AttributeComp->TakeDamageAmount(0.f);
			ImpactEffect(GetActorLocation());

			CombatInterface->Parried();

			AttributeComp->ToggleStaminaRegen(true, 1.f);

		}
		return ActualDamage;
	}

	if (CanAttackBlocking())
	{
		AttributeComp->ToggleStaminaRegen(false);
		AttributeComp->TakeDamageAmount(0.5f);
		StateComp->SetState(BSGameplayTag::Character_Action_BlockingHit);
	}
	else
	{
		AttributeComp->TakeDamageAmount(ActualDamage);
		StateComp->SetState(BSGameplayTag::Character_State_Hit);
	}

	StateComp->ToggleMovementInput(false);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		FVector ShotDirection = PointDamageEvent->ShotDirection;
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;
		FVector HitLocation = PointDamageEvent->HitInfo.Location;

		ImpactEffect(ImpactPoint);

		HitReaction(EventInstigator->GetPawn(), EDamageType::HitBack);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		const FRadialDamageEvent* RadialDamageEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);

		const FVector HitLocation = RadialDamageEvent->Origin;

		ImpactEffect(HitLocation);

		HitReaction(EventInstigator->GetPawn(), EDamageType::KnockBack);
	}

	return ActualDamage;
}

bool ABSCharacterPlayer::IsDead() const
{
	check(StateComp);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Death);

	return StateComp->IsCurrentStateEqualToAny(CheckTags);
}

void ABSCharacterPlayer::OnDeath()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetSimulatePhysics(true);
}

void ABSCharacterPlayer::ImpactEffect(const FVector& Location)
{
	check(CombatComp);

	if (CanAttackBlocking())
	{
		BlockImpactEffect(Location);
	}
	else
	{
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
		}

		if (ImpactParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Location);
		}
	}
}

void ABSCharacterPlayer::BlockImpactEffect(const FVector& Location)
{
	if (BlockingImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlockingImpactSound, Location);
	}

	if (BlockingImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BlockingImpactParticle, Location);
	}
}

void ABSCharacterPlayer::HitReaction(const AActor* Attacker, const EDamageType& DamageType)
{
	check(AttributeComp);
	check(CombatComp);

	if (ABSWeapon* MainWeapon = CombatComp->GetMainWeapon())
	{
		if (CanAttackBlocking())
		{
			if (UAnimMontage* BlockingHitReactAnimMontage = MainWeapon->GetMontageForTag(BSGameplayTag::Character_Action_BlockingHit))
			{
				AttributeComp->DecreaseStamina(20.f);
				AttributeComp->ToggleStaminaRegen(true, 1.f);
				PlayAnimMontage(BlockingHitReactAnimMontage);
			}
		}
		else
		{
			switch (DamageType)
			{
			case EDamageType::HitBack:
				if (UAnimMontage* HitReactAnimMontage = MainWeapon->GetHitReactMontage(Attacker))
				{
					PlayAnimMontage(HitReactAnimMontage);
				}
				break;
			case EDamageType::KnockBack:
				if (UAnimMontage* HitReactAnimMontage = MainWeapon->GetMontageForTag(BSGameplayTag::Character_Action_KnockBackHit))
				{
					PlayAnimMontage(HitReactAnimMontage);
				}
			}
		}
	}
}

bool ABSCharacterPlayer::IsMoving() const
{
	if (GetCharacterMovement()->Velocity.Size2D() > 3.f && GetCharacterMovement()->GetCurrentAcceleration() != FVector::ZeroVector)
	{
		return true;
	}
	return false;
}

bool ABSCharacterPlayer::CanRolling() const
{
	check(StateComp);
	check(AttributeComp);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Attacking);
	CheckTags.AddTag(BSGameplayTag::Character_State_GeneralAction);
	CheckTags.AddTag(BSGameplayTag::Character_State_Aiming);
	CheckTags.AddTag(BSGameplayTag::Character_State_Hit);
	CheckTags.AddTag(BSGameplayTag::Character_State_Rolling);
	CheckTags.AddTag(BSGameplayTag::Character_State_Blocking);

	return StateComp->IsCurrentStateEqualToAny(CheckTags) == false
		&& AttributeComp->CheckHasEnoughStamina(5.f);
}

bool ABSCharacterPlayer::CanChangeWeapon() const
{
	check(StateComp);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Attacking);
	CheckTags.AddTag(BSGameplayTag::Character_State_Rolling);
	CheckTags.AddTag(BSGameplayTag::Character_State_GeneralAction);
	CheckTags.AddTag(BSGameplayTag::Character_State_Hit);
	CheckTags.AddTag(BSGameplayTag::Character_State_Aiming);
	CheckTags.AddTag(BSGameplayTag::Character_State_Death);
	CheckTags.AddTag(BSGameplayTag::Character_State_Blocking);

	return StateComp->IsCurrentStateEqualToAny(CheckTags) == false;
}

bool ABSCharacterPlayer::CanBlockingStance() const
{
	check(AttributeComp);
	check(StateComp);

	if (bIsSprinting) return false;

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Attacking);
	CheckTags.AddTag(BSGameplayTag::Character_State_GeneralAction);
	CheckTags.AddTag(BSGameplayTag::Character_State_Hit);
	CheckTags.AddTag(BSGameplayTag::Character_State_Rolling);

	return StateComp->IsCurrentStateEqualToAny(CheckTags) == false
		&& AttributeComp->CheckHasEnoughStamina(1.f);
}

bool ABSCharacterPlayer::CanAttackBlocking() const
{
	check(CombatComp);
	check(AttributeComp);

	return bFacingEnemy && CombatComp->IsBlockingEnabled() && AttributeComp->CheckHasEnoughStamina(20.f);
}

bool ABSCharacterPlayer::CanParrying() const
{
	return bFacingEnemy && bParryEnabled && CanBlockingStance();
}

bool ABSCharacterPlayer::CanDetectForBackAttack() const
{
	check(StateComp);
	check(CombatComp);

	if (CombatComp->GetMainWeapon() == nullptr)
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Attacking);
	CheckTags.AddTag(BSGameplayTag::Character_State_Aiming);
	CheckTags.AddTag(BSGameplayTag::Character_State_Hit);
	CheckTags.AddTag(BSGameplayTag::Character_State_Death);
	CheckTags.AddTag(BSGameplayTag::Character_State_Rolling);
	CheckTags.AddTag(BSGameplayTag::Character_State_Parrying);

	return StateComp->IsCurrentStateEqualToAny(CheckTags) == false;
}

void ABSCharacterPlayer::Move(const FInputActionValue& Value)
{
	check(StateComp);
	check(RotationComp);

	if (RotationComp->GetIsActive())
	{
		FVector2D InputValue = Value.Get<FVector2D>();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector FinalDiection = (ForwardDirection * InputValue.X) + (RightDirection * InputValue.Y);

		RotationComp->SetRotationPlayer(FinalDiection);
	}

	if (!StateComp->GetMovementInputEnabled())
	{
		return;
	}

	FVector2D InputValue = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputValue.X);
	AddMovementInput(RightDirection, InputValue.Y);
}

void ABSCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(-InputValue.Y);
}

void ABSCharacterPlayer::Sprint()
{
	check(AttributeComp);
	check(CombatComp);

	if (bProgressAiming) return;

	if (CombatComp->IsBlockingEnabled())
	{
		return;
	}

	if (AttributeComp->CheckHasEnoughStamina(3.f) && IsMoving())
	{
		AttributeComp->ToggleStaminaRegen(false);

		AttributeComp->DecreaseStamina(0.1f);

		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

		bIsSprinting = true;
	}
	else
	{
		StopSprint();
	}
}

void ABSCharacterPlayer::StopSprint()
{
	check(AttributeComp);
	check(CombatComp);

	if (CombatComp->IsBlockingEnabled())
	{
		return;
	}

	AttributeComp->ToggleStaminaRegen(true);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bIsSprinting = false;
}

void ABSCharacterPlayer::Rolling()
{
	check(CombatComp);

	if (CanRolling())
	{
		if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
		{
			StateComp->ToggleMovementInput(false);

			AttributeComp->ToggleStaminaRegen(false);
			AttributeComp->DecreaseStamina(5.f);

			PlayAnimMontage(Weapon->GetMontageForTag(BSGameplayTag::Character_State_Rolling));

			StateComp->SetState(BSGameplayTag::Character_State_Rolling);

			AttributeComp->ToggleStaminaRegen(true, 1.5f);
		}
	}
}

void ABSCharacterPlayer::ToggleInventory()
{
	InventoryComp->ToggleInventory();
}

void ABSCharacterPlayer::TogglePlayerStatus()
{
	if (PlayerStatusWidget)
	{
		if (PlayerStatusWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			GetPlayerController()->SetInputModeGameOnly();

			PlayerStatusWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			GetPlayerController()->SetInputModeUIAndGame();

			PlayerStatusWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABSCharacterPlayer::Interaction()
{
	FHitResult HitResult;

	const FVector Start = GetActorLocation();
	const FVector End = Start;
	const float Radius = 100.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACTION));

	TArray<AActor*> ActorsToIgnore;

	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		Start,
		End,
		Radius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (bHit)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (IBSInteractInterface* InteractInterface = Cast<IBSInteractInterface>(HitActor))
			{
				InteractInterface->Interact(this);
			}
		}
	}

}

void ABSCharacterPlayer::ChangeWeapon()
{
	check(CombatComp);
	check(StateComp);

	if (CombatComp->CanChangeWeapon() && CanChangeWeapon())
	{
		if (UAnimMontage* UnequipMontage = CombatComp->GetMainWeapon()->GetMontageForTag(BSGameplayTag::Character_Action_Unequip))
		{
			StateComp->SetState(BSGameplayTag::Character_State_GeneralAction);

			PlayAnimMontage(UnequipMontage);
		}
	}
}

void ABSCharacterPlayer::LightAttack()
{
	FGameplayTag AttackType = GetAttackPerform();

	if (CanPerformAttack(AttackType) && bAiming == false)
	{
		ExecuteComboAttack(AttackType);
	}
	else if (bAiming)
	{
		FireArrow(AttackType);
	}
}

void ABSCharacterPlayer::HeavyAttack()
{
	FGameplayTag AttackType = BSGameplayTag::Character_Attack_Heavy;

	if (CanPerformAttack(AttackType))
	{
		ExecuteComboAttack(AttackType);
	}
}

void ABSCharacterPlayer::SpecialAttack()
{
	FGameplayTag AttackType = BSGameplayTag::Character_Attack_Special;

	if (CanPerformAttack(AttackType))
	{
		ExecuteComboAttack(AttackType);
	}
}

void ABSCharacterPlayer::Blocking()
{
	check(CombatComp);
	check(StateComp);

	if (CanBlockingStance())
	{
		GetCharacterMovement()->MaxWalkSpeed = BlockSpeed;
		CombatComp->SetBlockingEnabled(true);
		if (IBSUpdateAnyTypeInterface* AnimInterface = Cast<IBSUpdateAnyTypeInterface>(GetMesh()->GetAnimInstance()))
		{
			AnimInterface->UpdateBlcokingState(true);

			GetWorld()->GetTimerManager().SetTimer(ParryStartTimer, this, &ThisClass::Parry, 1.f, false, 0.2f);

			GetWorld()->GetTimerManager().SetTimer(ParryEndTimer, this, &ThisClass::ParryEnd, 1.f, false, 0.8f);
		}
	}
}

void ABSCharacterPlayer::BlockingEnd()
{
	check(CombatComp);
	check(StateComp);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	CombatComp->SetBlockingEnabled(false);
	if (IBSUpdateAnyTypeInterface* AnimInterface = Cast<IBSUpdateAnyTypeInterface>(GetMesh()->GetAnimInstance()))
	{
		AnimInterface->UpdateBlcokingState(false);
		StateComp->ClearState();
	}
}

void ABSCharacterPlayer::Parry()
{
	check(StateComp);

	bParryEnabled = true;
	StateComp->SetState(BSGameplayTag::Character_State_Parrying);
}

void ABSCharacterPlayer::ParryEnd()
{
	check(CombatComp);
	check(StateComp);

	bParryEnabled = false;

	if (CombatComp->IsBlockingEnabled())
	{
		StateComp->SetState(BSGameplayTag::Character_State_Blocking);
	}
	else
	{
		StateComp->ClearState();
	}

	GetWorld()->GetTimerManager().ClearTimer(ParryStartTimer);
	GetWorld()->GetTimerManager().ClearTimer(ParryEndTimer);
}

FGameplayTag ABSCharacterPlayer::GetAttackPerform() const
{
	if (bIsSprinting)
	{
		return BSGameplayTag::Character_Attack_Running;
	}
	else if (bAiming)
	{
		return BSGameplayTag::Character_Attack_Fire;
	}
	else if (bBackAttack)
	{
		return BSGameplayTag::Character_Attack_BackAttack;
	}
	else if (bPostureAttack)
	{
		return BSGameplayTag::Character_Attack_MaxPostureAttack;
	}
	return BSGameplayTag::Character_Attack_Light;
}

bool ABSCharacterPlayer::CanPerformAttack(const FGameplayTag& AttackType)
{
	check(StateComp);
	check(CombatComp);
	check(AttributeComp);

	FGameplayTagContainer CheckedTags;
	CheckedTags.AddTag(BSGameplayTag::Character_State_Rolling);
	CheckedTags.AddTag(BSGameplayTag::Character_State_GeneralAction);
	CheckedTags.AddTag(BSGameplayTag::Character_State_Hit);
	CheckedTags.AddTag(BSGameplayTag::Character_State_Death);
	CheckedTags.AddTag(BSGameplayTag::Character_Action_BlockingHit);

	const float StaminaCost = CombatComp->GetMainWeapon()->GetStaminaCost(AttackType);

	return StateComp->IsCurrentStateEqualToAny(CheckedTags) == false
		&& CombatComp->IsCombatEnabled()
		&& AttributeComp->CheckHasEnoughStamina(StaminaCost);

}

void ABSCharacterPlayer::ExecuteComboAttack(const FGameplayTag& AttackType)
{
	check(StateComp);

	if (StateComp->GetCurrentState() != BSGameplayTag::Character_State_Attacking)
	{
		if (bComboSequenceRunning && bCanComboInput == false)
		{
			// 추가 콤보 입력 기회
			ComboCounter++;
		}
		else
		{
			ResetCombo();
			bComboSequenceRunning = true;
		}

		DoAttack(AttackType);
		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	}
	else if (bCanComboInput)
	{
		bSavedComboInput = true;
	}
}

void ABSCharacterPlayer::DoAttack(const FGameplayTag& AttackType)
{
	check(StateComp);
	check(CombatComp);
	check(AttributeComp);

	if (const ABSWeapon* Weapon = CombatComp->GetMainWeapon())
	{
		StateComp->SetState(BSGameplayTag::Character_State_Attacking);

		StateComp->ToggleMovementInput(false);

		AttributeComp->ToggleStaminaRegen(false);

		CombatComp->SetLastAttackType(AttackType);

		UAnimMontage* Montage = Weapon->GetMontageForTag(AttackType, ComboCounter);
		if (!Montage)
		{
			ComboCounter = 0;
			Montage = Weapon->GetMontageForTag(AttackType, ComboCounter);
		}

		if (AttackType == BSGameplayTag::Character_Attack_BackAttack)
		{
			if (IBSEnemyInterface* EnemyInterface = Cast<IBSEnemyInterface>(VitalAttackTarget))
			{
				EnemyInterface->ToggleBackAttackWidgetVisibility(false);
				EnemyInterface->BackAttacked(Weapon->GetMontageForTag(BSGameplayTag::Character_Action_BackAttackHit));
				bBackAttack = false;
				ResetCombo();
				BackAttackMotionWarp();
			}
		}
		else if (AttackType == BSGameplayTag::Character_Attack_MaxPostureAttack)
		{
			if (IBSEnemyInterface* EnemyInterface = Cast<IBSEnemyInterface>(VitalAttackTarget))
			{
				EnemyInterface->PostureAttacked(Weapon->GetMontageForTag(BSGameplayTag::Character_Action_MaxPostureHit));
				bPostureAttack = false;
				ResetCombo();
			}
		}
		PlayAnimMontage(Montage);

		const float StaminaCost = Weapon->GetStaminaCost(AttackType);

		AttributeComp->DecreaseStamina(StaminaCost);
		AttributeComp->ToggleStaminaRegen(true, 1.5f);
	}
}

void ABSCharacterPlayer::ResetCombo()
{
	bComboSequenceRunning = false;
	bCanComboInput = false;
	bSavedComboInput = false;
	ComboCounter = 0;
}

bool ABSCharacterPlayer::CanPullingString()
{
	check(StateComp);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_GeneralAction);
	CheckTags.AddTag(BSGameplayTag::Character_State_Attacking);
	CheckTags.AddTag(BSGameplayTag::Character_State_Death);
	CheckTags.AddTag(BSGameplayTag::Character_State_Rolling);

	return StateComp->IsCurrentStateEqualToAny(CheckTags) == false;
}

void ABSCharacterPlayer::PullString()
{
	check(CombatComp);

	if (IBSBowInterface* BowInterface = Cast<IBSBowInterface>(CombatComp->GetMainWeapon()))
	{
		bAiming = true;

		const FVector RightHandSocket = GetMesh()->GetSocketLocation(TEXT("RightHandSocket"));

		BowInterface->PullString(RightHandSocket);
	}
}


void ABSCharacterPlayer::PullStringStart()
{
	check(StateComp);
	check(CombatComp);

	if (CanPullingString())
	{
		if (const ABSWeapon* Weapon = CombatComp->GetMainWeapon())
		{
			ToggleAimingFlag(true);

			ToggleCameraViewAdjust();

			StateComp->SetState(BSGameplayTag::Character_State_Aiming);

			PlayAnimMontage(Weapon->GetMontageForTag(BSGameplayTag::Character_State_Aiming));
		}
	}
}

void ABSCharacterPlayer::PullStringCancel()
{
	check(StateComp);
	check(CombatComp);

	if (Arrow)
	{
		Arrow->Destroy();
	}

	if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			if (Anim->Montage_IsPlaying(Weapon->GetMontageForTag(BSGameplayTag::Character_State_Aiming)))
			{
				BowFireFinished();

				Anim->Montage_Stop(0.4f, Weapon->GetMontageForTag(BSGameplayTag::Character_State_Aiming));
			}

			if (IBSBowInterface* BowInterface = Cast<IBSBowInterface>(Weapon))
			{
				BowInterface->ReleaseString();
			}
		}
	}
}

void ABSCharacterPlayer::PullStringComplete()
{
	check(CombatComp);

	if (CanPullingString())
	{
		PullString();

		FActorSpawnParameters Param;
		Param.Owner = this;
		Param.Instigator = this;

		Arrow = GetWorld()->SpawnActor<ABSArrow>(ArrowClass, GetActorTransform(), Param);
		if (Arrow)
		{
			if (ABSWeaponBow* Bow = Cast<ABSWeaponBow>(CombatComp->GetMainWeapon()))
			{
				Arrow->AddIgnoreActor(this);
				Arrow->AddIgnoreActor(Bow);

				USceneComponent* DownCasting = Cast<USceneComponent>(Bow->BowMeshComp);
				Arrow->AttachToComponent(DownCasting, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("ArrowSocket"));
			}
		}
	}
}

void ABSCharacterPlayer::FireArrow(const FGameplayTag& AttackType)
{
	check(StateComp);
	check(AttributeComp);
	check(CombatComp);
	check(Arrow);

	if (AttributeComp->CheckHasEnoughStamina(5.f) == false)
	{
		PullStringCancel();
	}

	if (ABSWeapon* Weapon = CombatComp->GetMainWeapon())
	{
		AttributeComp->ToggleStaminaRegen(false);
		AttributeComp->DecreaseStamina(5.f);

		StateComp->ToggleMovementInput(false);
		StateComp->SetState(AttackType);

		PlayAnimMontage(Weapon->GetMontageForTag(AttackType));

		Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Arrow->Fire(CalculateViewDirection());

		AttributeComp->ToggleStaminaRegen(true, 1.5f);

		if (IBSBowInterface* BowInterface = Cast<IBSBowInterface>(Weapon))
		{
			BowInterface->ReleaseString();
		}
	}
}

FVector ABSCharacterPlayer::CalculateViewDirection() const
{
	FRotator ControlRot = Controller->GetControlRotation();

	FVector ControlDir = ControlRot.Vector();

	return ControlDir;
}

void ABSCharacterPlayer::ToggleCameraViewAdjust()
{
	if (bProgressAiming)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = 250.f;

		bUseControllerRotationYaw = true;

		SpringArmComp->bUsePawnControlRotation = true;
		SpringArmComp->TargetArmLength = AimingSpringArmLength;
		SpringArmComp->SetRelativeLocation(FVector(0.f, 60.f, 80.f));
		SpringArmComp->SetRelativeRotation(FRotator(0.f, -30.f, 0.f));
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		bUseControllerRotationYaw = false;

		SpringArmComp->bUsePawnControlRotation = true;
		SpringArmComp->TargetArmLength = BaseSpringArmLength;
		SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
		SpringArmComp->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	}
}

void ABSCharacterPlayer::ToggleAimingFlag(bool InIsAiming)
{
	bProgressAiming = InIsAiming;

	if (UBSAnimInstance* Anim = Cast<UBSAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		Anim->bIsAiming = bProgressAiming;
	}
}

bool ABSCharacterPlayer::DetectForBackAttackTarget(FHitResult& OutResult)
{
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * 200.f;
	FCollisionObjectQueryParams TraceParam;
	TraceParam.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	FCollisionQueryParams IgnoreParam;
	IgnoreParam.AddIgnoredActor(this);

	return GetWorld()->LineTraceSingleByObjectType(OutResult, Start, End, TraceParam, IgnoreParam);
}

void ABSCharacterPlayer::BackAttackMotionWarp()
{
	const FVector Target = VitalAttackTarget->GetActorLocation() + (VitalAttackTarget->GetActorForwardVector() * -1) * 100.f;
	const FRotator TargetRotation = VitalAttackTarget->GetActorRotation();
	MotionWarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("BackAttackLoc"), Target, TargetRotation);
	VitalAttackTarget = nullptr;
}

void ABSCharacterPlayer::ChagnedWeapon(const FInventorySlot&)
{
	check(CombatComp);

	if (ABSWeapon* CurrnetWeapon = CombatComp->GetMainWeapon())
	{
		SetInputMapping(CurrnetWeapon->GetWeaponType());
	}
	else
	{
		SetInputMapping(EWeaponType::Uncombat);
	}
}

void ABSCharacterPlayer::SetInputMapping(const EWeaponType& InWeaponType)
{
	if (InputMap.Contains(InWeaponType))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMap[InWeaponType], 0);
		}
	}
}

ABSPlayerController* ABSCharacterPlayer::GetPlayerController() const
{
	return CastChecked<ABSPlayerController>(GetController());
}



