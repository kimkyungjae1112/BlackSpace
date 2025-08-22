# Project : BlackSpace

- 개발 기간 : 2025.07.02 ~ 2025.08.22
- 개발 인원 : 1인
- 장르 : 소울라이크 + RPG

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









