// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define COLLISION_OBJECT_INTERACTION ECC_GameTraceChannel1
#define COLLISION_OBJECT_TARGETING ECC_GameTraceChannel2

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	Stamina,
	Health,
	Posture
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front,
	Back,
	Right,
	Left
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Uncombat,
	Sword,
	Polearm,
	Bow,
	Blunt,
	GreateSword,
	Axe,
};

UENUM(BlueprintType)
enum class EWeaponGrade : uint8
{
	Common,
	Rare,
	Epic,
	Legendary,
};

UENUM(BlueprintType)
enum class EWeaponCollisionType : uint8
{
	First,
	Second
};

UENUM(BlueprintType)
enum class EAIBehavior : uint8
{
	Idle,
	Patrol,
	MeleeAttack,
	RangedAttack,
	Approach,
	Stunned,
	Strafe,
	Blocking,
	Dodging,
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	HitBack,
	KnockBack,
};

UENUM(BlueprintType)
enum class ESwitchingDirection : uint8
{
	None,
	Left,
	Right,
};