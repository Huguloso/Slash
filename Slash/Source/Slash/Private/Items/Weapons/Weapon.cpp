#include "Items/Weapons/Weapon.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName SocketName)
{
	ItemMesh->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	ItemState = EItemState::EIS_Equipped;
}

void AWeapon::Equip(USceneComponent* InParent, FName SocketName)
{
	AttachMeshToSocket(InParent, SocketName);
	
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
