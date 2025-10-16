#include "Characters/BaseCharacter.h"

#include "AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->ClearIgnoreActorsArray();
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Attack(const FInputActionValue& Value)
{
}

void ABaseCharacter::PlayAttackMontage()
{
}

bool ABaseCharacter::CanAttack() const
{
	return true;
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName) const
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

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint) const
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


