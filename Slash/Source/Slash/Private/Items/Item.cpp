// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(0, 0, 50));
	SetActorRotation(FRotator(0, 45.f, 0));

}

float AItem::TransformedSin() const
{
	return FMath::Sin(RunningTime * TimeConstant) * Amplitude;
}

float AItem::TransformedCos() const
{
	return FMath::Cos(RunningTime * TimeConstant) * Amplitude;
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	
	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_LINE_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100);
}

