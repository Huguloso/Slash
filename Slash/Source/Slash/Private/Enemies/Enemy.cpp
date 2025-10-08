#include "Enemies/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = FVector(ImpactLowered - GetActorLocation()).GetSafeNormal();
	
	const FVector Front = FVector(GetActorForwardVector()).GetSafeNormal();
	const FVector Right = FVector(GetActorRightVector()).GetSafeNormal();
	const FVector Left = -Right;
	const FVector Back = -Front;

	const float FrontAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Front, ToHit)));
	const float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Right, ToHit)));
	const float LeftAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Left, ToHit)));
	const float BackAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Back, ToHit)));

	FName SectionName;
	
	if (FrontAngle >= -45.f && FrontAngle <= 45.f)
	{
		SectionName = FName("FromFront");
	}
	else if (RightAngle >= -45.f && RightAngle <= 45.f)
	{
		SectionName = FName("FromRight");
	}
	else if (LeftAngle >= -45.f && LeftAngle <= 45.f)
	{
		SectionName = FName("FromLeft");
	}
	else if (BackAngle >= -45.f && BackAngle <= 45.f)
	{
		SectionName = FName("FromBack");
	}
	
	PlayHitReactMontage(SectionName);
}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DirectionalHitReact(ImpactPoint);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, ImpactPoint);
	}
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (HitReactMontage)
		{
			AnimInstance->Montage_Play(HitReactMontage);
			AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
		}
	}
}

