// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/Slash.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	DRAW_SPHERE(GetActorLocation());
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

