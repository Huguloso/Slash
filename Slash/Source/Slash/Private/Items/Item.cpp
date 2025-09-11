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

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	float DeltaZ = FMath::Sin(RunningTime * TimeConstant) * Amplitude;
	
	AddActorWorldOffset(FVector(0, 0, DeltaZ));
		
	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_LINE_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100);
}

