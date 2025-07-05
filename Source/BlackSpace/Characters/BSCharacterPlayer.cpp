// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BSCharacterPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimMontage.h"

#include "Data/BSInputData.h"
#include "BSGameplayTag.h"
#include "Equipments/BSWeapon.h"
#include "Components/BSAttributeComponent.h"
#include "Components/BSInventoryComponent.h"
#include "Components/BSCombatComponent.h"
#include "Components/BSStateComponent.h"
#include "UI/BSPlayerHUDWidget.h"
#include "UI/BSPlayerStatusWidget.h"
#include "Interface/BSInteractInterface.h"
#include "Interface/BSBowInterface.h"
#include "Player/BSPlayerController.h"
#include "Animation/BSAnimInstance.h"

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
	InventoryComp = CreateDefaultSubobject<UBSInventoryComponent>(TEXT("Inventory Component"));
	CombatComp = CreateDefaultSubobject<UBSCombatComponent>(TEXT("Combat Component"));
	StateComp = CreateDefaultSubobject<UBSStateComponent>(TEXT("State Component"));

	CombatComp->OnChangedMainWeapon.AddUObject(this, &ThisClass::ChagnedWeapon);
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

	/*GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Stamina : %f"), AttributeComp->GetBaseStamina()));
	GEngine->AddOnScreenDebugMessage(1, 1.5f, FColor::Cyan, FString::Printf(TEXT("MaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed));*/
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
		EnhancedInputComponent->BindAction(InputData->IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(InputData->IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(InputData->IA_Sprint, ETriggerEvent::Triggered, this, &ThisClass::Sprint);
		EnhancedInputComponent->BindAction(InputData->IA_Sprint, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
		EnhancedInputComponent->BindAction(InputData->IA_ToggleInventory, ETriggerEvent::Started, this, &ThisClass::ToggleInventory);
		EnhancedInputComponent->BindAction(InputData->IA_TogglePlayerStatus, ETriggerEvent::Started, this, &ThisClass::TogglePlayerStatus);
		EnhancedInputComponent->BindAction(InputData->IA_Interact, ETriggerEvent::Started, this, &ThisClass::Interaction);

		/* Sword */
		EnhancedInputComponent->BindAction(InputData->IA_SwordAttack, ETriggerEvent::Canceled, this, &ThisClass::LightAttack);
		EnhancedInputComponent->BindAction(InputData->IA_SwordAttack, ETriggerEvent::Triggered, this, &ThisClass::SpecialAttack);
		EnhancedInputComponent->BindAction(InputData->IA_SwordHeavyAttack, ETriggerEvent::Started, this, &ThisClass::HeavyAttack);

		/* Bow */
		EnhancedInputComponent->BindAction(InputData->IA_BowStringPull, ETriggerEvent::Started, this, &ThisClass::PullStringStart);
		EnhancedInputComponent->BindAction(InputData->IA_BowStringPull, ETriggerEvent::Ongoing, this, &ThisClass::PullString);
		EnhancedInputComponent->BindAction(InputData->IA_BowStringPull, ETriggerEvent::Canceled, this, &ThisClass::PullStringCancel);
		EnhancedInputComponent->BindAction(InputData->IA_BowStringPull, ETriggerEvent::Completed, this, &ThisClass::PullStringComplete);
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

void ABSCharacterPlayer::AttackFinished(const float ComboResetDelay)
{
	check(StateComp);

	StateComp->ToggleMovementInput(true);

	// ComboResetDelay 후에 콤보 시퀀스 완전 종료
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ThisClass::ResetCombo, ComboResetDelay, false);
}

bool ABSCharacterPlayer::IsMoving() const
{
	if (GetCharacterMovement()->Velocity.Size2D() > 3.f && GetCharacterMovement()->GetCurrentAcceleration() != FVector::ZeroVector)
	{
		return true;
	}
	return false;
}

void ABSCharacterPlayer::Move(const FInputActionValue& Value)
{
	check(StateComp);
	if (!StateComp->GetMovementInputEnabled()) return;

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
	if (bProgressAiming) return;

	check(AttributeComp);

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

	AttributeComp->ToggleStaminaRegen(true);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bIsSprinting = false;
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
	UE_LOG(LogTemp, Display, TEXT("Interact"));
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
	CheckedTags.AddTag(BSGameplayTag::Character_State_Death);

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

void ABSCharacterPlayer::PullString()
{
	check(CombatComp);

	// 다른 상태에서 우클릭 시 활 시위 당겨지는 것 방지
	if (CanPullingString())
	{
		if (IBSBowInterface* BowInterface = Cast<IBSBowInterface>(CombatComp->GetMainWeapon()))
		{
			const FVector RightHandSocket = GetMesh()->GetSocketLocation(TEXT("RightHandSocket"));

			BowInterface->PullString(RightHandSocket);
		}
	}
}

void ABSCharacterPlayer::PullStringCancel()
{
	check(StateComp);
	check(CombatComp);

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
	bAiming = true;

	// 화살 발사 준비 특수효과 추가..
}

void ABSCharacterPlayer::FireArrow(const FGameplayTag& AttackType)
{
	check(StateComp);
	check(AttributeComp);
	check(CombatComp);

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

		AttributeComp->ToggleStaminaRegen(true, 1.5f);
	
		if (IBSBowInterface* BowInterface = Cast<IBSBowInterface>(Weapon))
		{
			BowInterface->ReleaseString();
		}
	}
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



