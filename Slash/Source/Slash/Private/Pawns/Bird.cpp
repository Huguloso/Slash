#include "Pawns/Bird.h"

#include "Components/CapsuleComponent.h"
#include "Slate/SGameLayerManager.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleHalfHeight(20);
	CapsuleComponent->SetCapsuleRadius(15);
	SetRootComponent(CapsuleComponent);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();
}

void ABird::MoveForward(float Value)
{
	
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}