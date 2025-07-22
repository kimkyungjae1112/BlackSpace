// Fill out your copyright notice in the Description page of Project Settings.


#include "BSGameplayTag.h"

namespace BSGameplayTag
{
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Attacking, "Character.State.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Rolling, "Character.State.Rolling");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_GeneralAction, "Character.State.GeneralAction");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Death, "Character.State.Death");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Aiming, "Character.State.Aiming");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Hit, "Character.State.Hit");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Blocking, "Character.State.Blocking");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Parrying, "Character.State.Parrying");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Parried, "Character.State.Parried");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_BackAttacked, "Character.State.BackAttacked");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Stunned, "Character.State.Stunned");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_MaxPosture, "Character.State.MaxPosture");

	UE_DEFINE_GAMEPLAY_TAG(Character_Action_Equip, "Character.Action.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_Unequip, "Character.Action.Unequip");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_HitReaction, "Character.Action.HitReaction");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_BlockingHit, "Character.Action.BlockingHit");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_ParriedHit, "Character.Action.ParriedHit");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_BackAttackHit, "Character.Action.BackAttackHit");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_KnockBackHit, "Character.Action.KnockBackHit");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_MaxPostureHit, "Character.Action.MaxPostureHit");

	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Light, "Character.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Heavy, "Character.Attack.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Running, "Character.Attack.Running");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Special, "Character.Attack.Special");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_BackAttack, "Character.Attack.BackAttack");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_MaxPostureAttack, "Character.Attack.MaxPostureAttack");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_ParryingAttack, "Character.Attack.ParryingAttack");

	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Fire, "Character.Attack.Fire");
}