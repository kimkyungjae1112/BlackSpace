// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/BSArrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABSArrow::ABSArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComp->bAutoActivate = false;
	ProjectileMovementComp->InitialSpeed = 2000.f;
	ProjectileMovementComp->MaxSpeed = 2000.f;
	ProjectileMovementComp->UpdatedComponent = RootComp;
	ProjectileMovementComp->bRotationFollowsVelocity = true;

	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
}

void ABSArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHit == false)
	{
		CollisionTrace();
	}
}

void ABSArrow::AddIgnoreActor(AActor* Actor)
{
	IgnoredActors.Add(Actor);
}

void ABSArrow::RemoveIgnoreActor(AActor* Actor)
{
	IgnoredActors.Remove(Actor);
}

void ABSArrow::Fire(const FVector& InDirection)
{
	ProjectileMovementComp->Velocity = InDirection * ProjectileMovementComp->InitialSpeed;
	ProjectileMovementComp->Activate();
	SetActorRotation(InDirection.Rotation());
}

void ABSArrow::CollisionTrace()
{
	FHitResult HitResult;

	const FVector ArrowHead = MeshComp->GetSocketLocation(ArrowHeadSocket);

	bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		ArrowHead,
		ArrowHead,
		TraceRadius,
		TraceObjectTypes,
		false,
		IgnoredActors,
		DrawDebugType,
		HitResult,
		true
	);

	if (bHit)
	{
		float AttackDamage = 50.f;

		const FVector DamageDirection = GetActorForwardVector();

		UGameplayStatics::ApplyPointDamage(
			HitResult.GetActor(),
			AttackDamage,
			DamageDirection,
			HitResult,
			GetOwner()->GetInstigatorController(),
			this,
			nullptr
		);

		Destroy();
	}
}

