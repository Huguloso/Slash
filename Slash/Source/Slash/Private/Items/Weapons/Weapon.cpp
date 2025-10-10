#include "Items/Weapons/Weapon.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	WeaponBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBoxComponent->SetupAttachment(GetRootComponent());
	WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	WeaponBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName SocketName)
{
	ItemMesh->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	ItemState = EItemState::EIS_Equipped;
}

void AWeapon::Equip(USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);

	AttachMeshToSocket(InParent, SocketName);
	
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (EmbersEffect)
	{
		EmbersEffect->Deactivate();
	}
}

void AWeapon::ClearIgnoreActorsArray()
{
	IgnoreActors.Empty();
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult Hit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f),
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		Hit,
		true);

	if (Hit.GetActor())
	{
		if (IHitInterface* HitInterface = Cast<IHitInterface>(Hit.GetActor()))
		{
			HitInterface->Execute_GetHit(Hit.GetActor(), Hit.ImpactPoint);
		}

		IgnoreActors.AddUnique(Hit.GetActor());

		CreateFields(Hit.ImpactPoint);
		UGameplayStatics::ApplyDamage(
			Hit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			GetOwner(),
			UDamageType::StaticClass());
	}
}
