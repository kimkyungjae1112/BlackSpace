// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/BSKnightRangedAttackProjectile.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"


ABSKnightRangedAttackProjectile::ABSKnightRangedAttackProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::BeginOverlap);

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	NiagaraComp->SetupAttachment(BoxComp);
	NiagaraComp->bAutoActivate = true;
}

void ABSKnightRangedAttackProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSKnightRangedAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (Direction * 1500.f * DeltaTime);
	SetActorLocation(NewLocation);

	FRotator NewRotation = GetActorRotation() + (FRotator(DeltaTime * 100.f, 0.f, 0.f));
	SetActorRotation(NewRotation);

	LifeTime -= DeltaTime;
	if (LifeTime < 0.f)
	{
		Destroy();
	}
}

void ABSKnightRangedAttackProjectile::SetDirection(const FVector& InDirection)
{
	Direction = InDirection;
}

void ABSKnightRangedAttackProjectile::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		UGameplayStatics::ApplyPointDamage(
			OtherActor,
			Damage,
			GetActorForwardVector(),
			FHitResult(),
			GetOwner()->GetInstigatorController(),
			this,
			nullptr
		);
	}
}

