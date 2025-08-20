// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/BSBossStageWall.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"

ABSBossStageWall::ABSBossStageWall()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	SetRootComponent(BoxComp);
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	NiagaraComp1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara1 Comp"));
	NiagaraComp1->SetupAttachment(BoxComp);
	NiagaraComp1->bAutoActivate = true;

	NiagaraComp2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara2 Comp"));
	NiagaraComp2->SetupAttachment(BoxComp);
	NiagaraComp2->bAutoActivate = true;
}
