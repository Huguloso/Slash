#include "Items/Item.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
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
}

