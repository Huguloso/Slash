#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	void AttachMeshToSocket(USceneComponent* InParent, FName SocketName);
	void Equip(USceneComponent* InParent, FName SocketName);
	void ClearIgnoreActorsArray();

	FORCEINLINE TObjectPtr<UBoxComponent> GetWeaponBox() const { return WeaponBoxComponent; }
protected:
	virtual void BeginPlay() override;
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> EquipSound;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> WeaponBoxComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceEnd;
	
	TArray<TObjectPtr<AActor>> IgnoreActors;
};
