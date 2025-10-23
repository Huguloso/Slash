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
	
	virtual void Destroyed() override;

	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,AActor* DamageCauser) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void Die() override;

	virtual bool CanAttack() const override;
	virtual void HandleDamage(float DamageAmount) override;
	
	virtual void Attack() override;

	virtual void PlayAttackMontage() override;
	
	bool InTargetRange(const AActor* Target, double Radius) const;
	
	void PatrolTimerFinished() const;

	void MoveToTarget(AActor* Target) const;

	AActor* ChoosePatrolTarget() const;

	UFUNCTION()
	void PawnSeen(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
private:
	void CheckCombatTarget();
	void CheckPatrolTarget();

	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius() const;
	bool IsOutsideAttackRadius() const;
	bool IsInsideAttackRadius() const;
	bool IsChasing() const;
	bool IsAttacking() const;
	bool IsDead() const;
	bool IsEngaged() const;
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
 
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1;
	
	UPROPERTY(EditAnywhere)
	double CombatRadius = 750;
	
	UPROPERTY(EditAnywhere)
	double AttackRadius = 130;
	
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> CurrentPatrolTarget;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<TObjectPtr<AActor>> PatrolTargets;
	
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double WaitMin = 4;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double WaitMax = 8;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrollingSpeed = 125;
	
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float ChasingSpeed = 300;
	
	UPROPERTY()
	TObjectPtr<AAIController> AIController;

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;
};
