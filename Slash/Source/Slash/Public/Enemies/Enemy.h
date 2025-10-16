#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;
class UAIPerceptionComponent;
class UHealthBarComponent;
class AAIController;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,AActor* DamageCauser) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void Die() override;

	bool InTargetRange(const AActor* Target, double Radius) const;
	
	void PatrolTimerFinished() const;

	void MoveToTarget(AActor* Target) const;

	AActor* ChoosePatrolTarget() const;

	UFUNCTION()
	void PawnSeen(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

private:
	void CheckCombatTarget();
	void CheckPatrolTarget();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
 
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 750;
	
	UPROPERTY(EditAnywhere)
	double AttackRadius = 150;
	
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> CurrentPatrolTarget;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<TObjectPtr<AActor>> PatrolTargets;
	
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double WaitMin = 4;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double WaitMax = 8;

	UPROPERTY()
	TObjectPtr<AAIController> AIController;

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;
	
	FTimerHandle PatrolTimer;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
};
