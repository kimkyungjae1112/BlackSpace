# Project : BlackSpace

- 개발 기간 : 2025.07.02 ~ 2025.08.22
- 개발 인원 : 1인
- 장르 : 소울라이크 + RPG
- 영상 : [유튜브 링크](https://youtu.be/vjcz6WjcW7I)

<br> 

적을 처치하고 무기를 얻어 강화시킨 후 최종 보스를 물리치는 것이 목표인 게임입니다.

<br><br>

# 개발 목표
- **코드 재사용**
  <br>
  기능을 컴포넌트화하여 공통 로직을 재사용하고 중복을 제거합니다.

<br>

- **디커플링**
  <br>
  한 클래스가 알아야 하는 코드·의존성을 최소화합니다.
  <br>
  델리게이트, 컴포넌트, 전역 시스템 등을 사용합니다.

<br>

- **OOP 원칙**
  <br>
  단일 책임과 역할 분리를 지키고, 상호작용은 인터페이스 우선으로 설계합니다.

<br>

- **주석 없이도 읽히는 코드**
  <br>
  의도가 드러나는 이름 짓기, 일관된 코딩 규칙, 작은 함수 단위로 주석 없이도 잘 읽히는 코드를 작성합니다.

<br>

- **전투 AI**
  <br>
  Behavior Tree, AIPerception, IGenericTeamAgentInterface 등을 이용해 실제 사람과 싸우는 듯한 의사결정을 구현합니다.

<br>

- **다양한 액션 제공**
  <br>
  패리/가드/카운터/위치 전술 등 상황별 선택지를 넓혀 전투 깊이를 높입니다.

<br>

# 컴포넌트 역할

### UBSStateComponent
- 플레이어, AI 캐릭터 모두 장착합니다.
- 캐릭터의 현재 상태와 이동 입력 허용 여부를 관리하고, 지연 후 입력 재활성 등 상태 초기화를 제공합니다.

<br>

### UBSRotationComponent
- 플레이어, AI 캐릭터 모두 장착합니다.
- 오너의 회전을 제어해 적 AI는 플레이어를 바라보게, 플레이어는 공격할 때 방향키를 사용해 공격 중간에 회전할 수 있습니다.
- 커스텀 NotifyState를 이용해서 공격 몽타주에서 활성/비활성을 설정합니다.

<br>

### UBSCombatComponent
- 플레이어, AI 캐릭터 모두 장착합니다.
- 메인/보조 무기 장비·교체·해제와 전투/가드 상태, 마지막 공격 태그를 관리하며 HUD에 변경사항 알림니다.

<br>

### UBSAttributeComponent
- 플레이어, AI 캐릭터 모두 장착합니다.
- 체력/스태미나/체간 값과 재생·피해 처리를 담당, 체력/스태미나/체간 변화·사망·체간 최대 도달을 델리게이트를 통해 알림니다.

<br>

### UBSTargetingComponent
- 플레이어만 장착합니다.
- 반경 내 타깃을 탐색·락온하고 카메라/컨트롤 회전을 대상에 맞춰 유지, 좌우 전환까지 관리합니다.

<br>

### UBSInventoryComponent
- 플레이어만 장착합니다.
- 무기를 저장할 컨테이너 관리 및 인벤토리 UI를 관리합니다.

<br>

### UBSDialogueComponent
- NPC만 장착합니다.
- 해당 컴포넌트를 장착하면 플레이어가 감지할 수 있으며, 데이터 테이블을 기반으로 대화할 수 있습니다.

<br>

### UBSWeaponCollisionComponent
- 모든 무기가 장착합니다.
- 무기 소켓 간 구간에 콜리전을 생성해 피격 대상 검출, 아군이나 중복 타격을 거른 후 브로드캐스트로 무기에 대미지를 입힐 액터를 전달합니다.

<br>

# 전투 시스템
- 공격·방어·회피는 캐릭터의 현재 상태에 따라 제한됩니다.
- 상태 판정을 일관되게 적용하기 위해 GameplayTag 기반의 상태 관리를 사용하며, 각 행동은 실행 전에 관련 상태 태그를 검사합니다. 
- 아래는 캐릭터가 가질 수 있는 상태 태그 목록입니다.

```
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Attacking);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Rolling);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_GeneralAction);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Death);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Aiming);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Hit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Blocking);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Parrying);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Parried);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_BackAttacked);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Stunned);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_MaxPosture);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_MaxPostureAttacked);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Dodge);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_Equip);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_Unequip);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_HitReaction);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_BlockingHit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_ParriedHit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_BackAttackHit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_KnockBackHit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_MaxPostureHit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Action_Dodge);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_Light);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_Heavy);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_Running);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_Special);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_BackAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_MaxPostureAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_ParryingAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_RangedAttack);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Attack_Fire);
```

<br>

- 공격 입력이 들어오면 먼저 플레이어가 공격을 할 수 있는 상태인지 확인합니다. 
- 공격 뿐 아니라 방어나 구르기 등 다른 입력에 관해서도 마찬가지 입니다.
```
// 공격을 할 수 있는지 검사하는 메서드
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
```

<br>

- 만약 공격을 할 수 있는 상황이라면 **GameplayTag**를 인자로 현재 장착한 무기에 전달하여 애니메이션을 실행합니다.
```
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
```

<br>

- 암살 공격, 화살 발사, 체간 공격 또한 **GameplayTag** 로 관리됩니다.
```
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
```

<br>

# 인벤토리 UI
- 인벤토리는 미리 공간을 할당받은 뒤 객체들을 배열에서 관리하는 객체 풀 방식을 사용합니다.
```
UBSInventoryComponent::UBSInventoryComponent()
{
	InventorySlots.SetNum(12);
	Weapons.SetNum(12);
}
```

<br>

- 무기를 획득하는 방법은 PickupItem 클래스로 만들어진 Blueprint와 상호작용 하는 것 입니다.
```
void ABSPickupItem::Interact(AActor* Interactor)
{
	if (EquipmentClass)
	{
		if (ABSEquipmentBase* CDO = EquipmentClass->GetDefaultObject<ABSEquipmentBase>())
		{
			if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			{
				if (UBSCombatComponent* CombatComp = Player->GetComponentByClass<UBSCombatComponent>())
				{
					if (CombatComp->GetMainWeapon() == nullptr || CombatComp->GetSecondaryWeapon() == nullptr)
					{
						FActorSpawnParameters Param;
						Param.Owner = Player;

						ABSEquipmentBase* Weapon = GetWorld()->SpawnActor<ABSEquipmentBase>(EquipmentClass, Player->GetActorTransform(), Param);
						if (Weapon)
						{
							Weapon->OnceCalledSetWeaponDamage();
							Weapon->EquipItem();
						}
					}
					else
					{
						if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
						{
							InventoryComp->AddToSlot(CDO->GetInventoryInfo());
						}
					}
				}
			}
		}
	}

	Destroy();
}
```

<br>

- 메인 무기와 보조 무기를 가지고 있는 상태일 때 인벤토리에 저장되기 시작합니다.
- **AddToSlot**은 다음과 같이 정의되어 있으며, 상황에 따라 오버로딩된 다른 함수들을 호출합니다.
```
void UBSInventoryComponent::AddToSlot(const FInventorySlot& InventorySlot)
{
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (InventorySlots[i].Quantity == 0)
		{
			FActorSpawnParameters Param;
			Param.Owner = GetOwner();

			ABSWeapon* Weapon = GetWorld()->SpawnActor<ABSWeapon>(InventorySlot.ItemClass, GetOwner()->GetActorTransform(), Param);
			Weapon->SetInventoryInfo(InventorySlot);
			Weapon->OnceCalledSetWeaponDamage();
			Weapon->SetActorHiddenInGame(true);
			Weapons[i] = Weapon;

			InventorySlots[i] = Weapon->GetInventoryInfo();

			if (OnInventoryUpdated.IsBound())
			{
				OnInventoryUpdated.Broadcast(InventorySlots);
			}

			break;
		}
	}
}
```

<br>

- 인벤토리 UI는 **UWidgetSwitcher**를 이용해서 플레이어 상태 창, 인벤토리 창,  조합 창을 넘어다니며 볼 수 있습니다.
- 인벤토리에 있는 슬롯들은 각각이 개별 클래스이며 아래의 함수들을 오버라이딩하여 UI 기능을 구현했습니다.
```
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent & InMouseEvent) override;
```

<br>

- 슬롯 위에서 우클릭을 할 때 무기 장착과 조합 슬롯에 무기를 넣는 두 가지 이벤트를 구분합니다.
```
FReply UBSInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		RightClickForEquip();
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UBSInventorySlotWidget::RightClickForEquip()
{
	if (InventorySlot.Quantity > 0)
	{
		APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (Player == nullptr) return;

		if (UBSInventoryMenuWidget::CurrentTabIndex == 2)
		{
			TArray<UUserWidget*> Widgets;
			TSubclassOf<UBSInventoryMenuWidget> InventoryWidgetClass = UBSInventoryMenuWidget::StaticClass();
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, InventoryWidgetClass, true);
			for (UUserWidget* Widget : Widgets)
			{
				UBSInventoryMenuWidget* InventoryMenuWidget = Cast<UBSInventoryMenuWidget>(Widget);
				
				InventoryMenuWidget->SetWeaponSlot(InventorySlot, Index);
				// 아래 함수들이 줄줄이 달려있음
				// UBSMixtureWeaponWidget->SetWeaponSlot()
				// UBSMixtureMaterialWidget->SetWeaponSlot()
				
				if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
				{
					InventoryComp->RemoveToSlot(Index);
				}

				break;
			}
			return;
		}

		if (UBSInventoryComponent* InventoryComp = Player->GetComponentByClass<UBSInventoryComponent>())
		{
			if (UBSCombatComponent* CombatComp = Player->GetComponentByClass<UBSCombatComponent>())
			{
				ABSWeapon* OldWeapon = nullptr;
				bool bIsHidden = true;
				if (CombatComp->CheckHasMainWeapon() && CombatComp->CheckHasSecondaryWeapon())
				{
					OldWeapon = CombatComp->GetMainWeapon();
					bIsHidden = false;
				}
				else if (!CombatComp->CheckHasMainWeapon())
				{
					bIsHidden = false;
				}
				InventoryComp->EquipFromInventory(Index, bIsHidden);
				InventoryComp->RemoveToSlot(Index);
				InventoryComp->AddToSlot(OldWeapon, Index);
			}
		}
	}
}
```

<br>

# 적 AI
- Service 노드를 통해 현재 AI의 상태를 계속해서 확인하고 그 상태에 맞는 행동을 하도록 설정합니다.
```
// 일반 적의 행동 결정 메서드
void UBTService_SelectBehavior::UpdateBehavior(UBlackboardComponent* BlackboardComp, ABSCharacterEnemy* ControlledEnemy) const
{
	check(BlackboardComp);
	check(ControlledEnemy);
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	UBSStateComponent* StateComp = ControlledEnemy->GetComponentByClass<UBSStateComponent>();
	check(StateComp);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Parried);
	CheckTags.AddTag(BSGameplayTag::Character_State_BackAttacked);
	CheckTags.AddTag(BSGameplayTag::Character_State_Stunned);
	CheckTags.AddTag(BSGameplayTag::Character_State_MaxPosture);
	CheckTags.AddTag(BSGameplayTag::Character_State_MaxPostureAttacked);

	if (StateComp->IsCurrentStateEqualToAny(CheckTags))
	{
		SetBehaviorKey(BlackboardComp, EAIBehavior::Stunned);
	}
	else
	{
		if (IsValid(TargetActor))
		{
			const float Distance = TargetActor->GetDistanceTo(ControlledEnemy);

			if (Distance <= AttackRangeDistance)
			{
				SetBehaviorKey(BlackboardComp, EAIBehavior::MeleeAttack);
			}
			else if (ControlledEnemy->GetPatrolPoint() && TargetActor->GetDistanceTo(Cast<AActor>(ControlledEnemy->GetPatrolPoint())) > ApprochRangeDistance)
			{
				SetBehaviorKey(BlackboardComp, EAIBehavior::Patrol);
			}
			else
			{
				SetBehaviorKey(BlackboardComp, EAIBehavior::Approach);
			}
		}
		else
		{
			if (ControlledEnemy->GetPatrolPoint() != nullptr)
			{
				SetBehaviorKey(BlackboardComp, EAIBehavior::Patrol);
			}
			else
			{
				SetBehaviorKey(BlackboardComp, EAIBehavior::Idle);
			}
		}
	}
}

// Maldrith 보스의 행동 결정 메서드
void UBTService_SelectBehaviorMaldrith::UpdateBehavior(UBlackboardComponent* BlackboardComp, ABSCharacterEnemy* ControlledEnemy) const
{
	check(BlackboardComp);
	check(ControlledEnemy);

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
	
	UBSStateComponent* StateComp = ControlledEnemy->GetComponentByClass<UBSStateComponent>();
	check(StateComp);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Parried);
	CheckTags.AddTag(BSGameplayTag::Character_State_Stunned);
	CheckTags.AddTag(BSGameplayTag::Character_State_MaxPosture);
	CheckTags.AddTag(BSGameplayTag::Character_State_MaxPostureAttacked);

	if (StateComp->IsCurrentStateEqualToAny(CheckTags))
	{
		SetBehaviorKey(BlackboardComp, EAIBehavior::Stunned);
	}
	else
	{
		if (IsValid(Target))
		{
			if (UBSAttributeComponent* AttributeComp = ControlledEnemy->GetComponentByClass<UBSAttributeComponent>())
			{
				if (AttributeComp->GetBaseStamina() <= StaminaCheckValue)
				{
					SetBehaviorKey(BlackboardComp, EAIBehavior::Strafe);
				}
				else
				{
					const float Distance = Target->GetDistanceTo(ControlledEnemy);

					if (Distance < AttackRangeDistance)
					{
						SetBehaviorKey(BlackboardComp, EAIBehavior::MeleeAttack);
					}
					else if (Distance >= AttackRangeDistance && RangedAttackRate)
					{
						SetBehaviorKey(BlackboardComp, EAIBehavior::RangedAttack);
					}
					else
					{
						SetBehaviorKey(BlackboardComp, EAIBehavior::Approach);
					}
				}
			}
		}
		else
		{
			SetBehaviorKey(BlackboardComp, EAIBehavior::Idle);
		}
	}
}

// Knight 보스의 행동 결정 메서드
void UBTService_SelectBehaviorKnight::UpdateBehavior(UBlackboardComponent* BlackboardComp, ABSCharacterEnemy* ControlledEnemy) const
{
	check(BlackboardComp);
	check(ControlledEnemy);

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	UBSStateComponent* StateComp = ControlledEnemy->GetComponentByClass<UBSStateComponent>();
	check(StateComp);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(BSGameplayTag::Character_State_Parried);
	CheckTags.AddTag(BSGameplayTag::Character_State_Stunned);
	CheckTags.AddTag(BSGameplayTag::Character_State_MaxPosture);
	CheckTags.AddTag(BSGameplayTag::Character_State_MaxPostureAttacked);

	if (StateComp->IsCurrentStateEqualToAny(CheckTags))
	{
		SetBehaviorKey(BlackboardComp, EAIBehavior::Stunned);
	}
	else
	{
		if (IsValid(Target) && StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_Blocking))
		{
			SetBehaviorKey(BlackboardComp, EAIBehavior::Blocking);
		}
		else if (IsValid(Target) && StateComp->IsCurrentStateEqualToIt(BSGameplayTag::Character_State_Dodge))
		{
			SetBehaviorKey(BlackboardComp, EAIBehavior::Dodging);
		}
		else if (IsValid(Target))
		{
			if (UBSAttributeComponent* AttributeComp = ControlledEnemy->GetComponentByClass<UBSAttributeComponent>())
			{
				if (AttributeComp->GetBaseStamina() <= StaminaCheckValue)
				{
					SetBehaviorKey(BlackboardComp, EAIBehavior::Strafe);
				}
				else
				{
					const float Distance = Target->GetDistanceTo(ControlledEnemy);

					if (Distance < AttackRangeDistance)
					{
						SetBehaviorKey(BlackboardComp, EAIBehavior::MeleeAttack);
					}
					else if (Distance >= AttackRangeDistance && RangedAttackRate)
					{
						SetBehaviorKey(BlackboardComp, EAIBehavior::RangedAttack);
					}
					else
					{
						SetBehaviorKey(BlackboardComp, EAIBehavior::Approach);
					}
				}
			}
		}
		else
		{
			SetBehaviorKey(BlackboardComp, EAIBehavior::Idle);
		}
	}
}

```





