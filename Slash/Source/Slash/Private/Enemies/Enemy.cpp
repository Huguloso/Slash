#include "Enemies/Enemy.h"

#include "AIController.h"
#include "AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

	AIController = Cast<AAIController>(GetController());

	if (AIController && CurrentPatrolTarget)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(CurrentPatrolTarget);
		MoveRequest.SetAcceptanceRadius(15);
		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveRequest, &NavPath);
		TArray<FNavPathPoint>& PathPoints= NavPath->GetPathPoints();
		for (FNavPathPoint& PathPoint : PathPoints)
		{
			const FVector& Location = PathPoint.Location;
			DrawDebugSphere(GetWorld(), Location, 10, 20, FColor::Green, false, 10);
		}
		
	}
}

void AEnemy::Die()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (DeathMontage)
		{
			AnimInstance->Montage_Play(DeathMontage);

			const int32 Selection = FMath::RandRange(0, 1);
			FName SectionName = FName();

			switch (Selection)
			{
			case 0:
				SectionName = FName("Death1");
				DeathPose = EDeathPose::EDP_Death1;
				break;
			case 1:
				SectionName = FName("Death2");
				DeathPose = EDeathPose::EDP_Death2;
				break;
			case 2:
				SectionName = FName("Death3");
				DeathPose = EDeathPose::EDP_Death3;
				break;
			case 3:
				SectionName = FName("Death4");
				DeathPose = EDeathPose::EDP_Death4;
				break;
			case 4:
				SectionName = FName("Death5");
				DeathPose = EDeathPose::EDP_Death5;
				break;
			default:
				break;
			}

			AnimInstance->Montage_JumpToSection(SectionName, DeathMontage); 
		}
	}

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		if (!InTargetRange(CombatTarget, CombatRadius))
		{
			CombatTarget = nullptr;
			if (HealthBarWidget)
			{
				HealthBarWidget->SetVisibility(false);
			}
		}
	}

	if (AIController && CurrentPatrolTarget)
	{
		TArray<AActor*> ValidTargets = PatrolTargets;
		ValidTargets.Remove(CurrentPatrolTarget);
		
		if (InTargetRange(CurrentPatrolTarget, PatrolRadius) && ValidTargets.Num() > 0)
		{
			const int32 Selection = FMath::RandRange(0, ValidTargets.Num() - 1);
			CurrentPatrolTarget = ValidTargets[Selection];
			FAIMoveRequest MoveRequest;
			
			MoveRequest.SetGoalActor(CurrentPatrolTarget);
			MoveRequest.SetAcceptanceRadius(15);
			
			AIController->MoveTo(MoveRequest);
		}
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,AActor* DamageCauser)
{
	if (AttributeComponent)
	{
		AttributeComponent->ReceiveDamage(Damage);
			
		if (HealthBarWidget)
		{
			HealthBarWidget->SetHealthPercent(AttributeComponent->GetHealthPercent());
		}
	}

	CombatTarget = EventInstigator->GetPawn();
	return Damage;
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

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
	
	if (AttributeComponent && AttributeComponent->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
	
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

bool AEnemy::InTargetRange(const AActor* Target, const double Radius) const
{
	const double Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
	return Distance <= Radius;
}

