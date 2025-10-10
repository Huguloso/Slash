#include "Breakable/BreakableActor.h"

#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(GeometryCollectionComponent);
	GeometryCollectionComponent->SetGenerateOverlapEvents(true);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (Broken)
	{
		return;
	}

	Broken = true;
	
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	if (GetWorld() && TreasureClasses.Num() > 0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75;

		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		GetWorld()->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}
}
