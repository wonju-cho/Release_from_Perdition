// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FlyingEnemyCharacter.generated.h"

class UBoxComponent;

UCLASS()
class SHADOWRUNNER_API AFlyingEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFlyingEnemyCharacter();

	// Character configurations.
	UPROPERTY(EditAnywhere, Category = "Configurations")
	float health;
	UPROPERTY(EditAnywhere, Category = "Configurations")
	float flySpeed;
	UPROPERTY(EditAnywhere, Category = "Configurations")
	float waveTurnRate;
	UPROPERTY(EditAnywhere, Category = "Configurations")
	float chargeSpeed;
	UPROPERTY(EditAnywhere, Category = "Configurations")
	float damage;

	float GetEnemyHealth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTree* behaviorTree;

	UPROPERTY(VisibleAnywhere, Category = AI)
	class UPawnSensingComponent* pawnSensingComponent;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AShadowRunnerProjectile> ProjectileClass;

	// Pickup drop.
	UPROPERTY(EditDefaultsOnly, Category = Drop)
	TSubclassOf<class AHealthPickupActor> healthDropClass;

	UPROPERTY(EditDefaultsOnly, Category = Drop)
	TSubclassOf<class AAmmoPickupActor> ammoDropClass;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* DieSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* ScreamSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* ExplodeSound;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* hitMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* baseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UParticleSystem* explodeParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* screamAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* chargeAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* moveAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* deathAnimation;

	// Timer handle for changing material.
	struct FTimerHandle hitTimerHandle;
	float hitDisplayTime;

	// Timer handle for death sequence.
	struct FTimerHandle deathTimerHandle;
	float deathAnimTime;
  
	UFUNCTION()
	void SpawnProjectile();

	UFUNCTION()
	void HitTimerCallback();

	UFUNCTION()
	void DeathTimerCallback();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void PlayScreamAnimation();

	UFUNCTION()
	void PlayChargeAnimation();

	UFUNCTION()
	void PlayMoveAnimation();

	UFUNCTION()
	void PlayDeathAnimation();

	UFUNCTION()
	void ResetAnimFlags();

	// Get configuration variables.
	UFUNCTION()
	float GetFlySpeed();
	UFUNCTION()
	float GetWaveTurnRate();
	UFUNCTION()
	float GetChargeSpeed();

	// Box collider.
	UPROPERTY(EditAnywhere)
	UBoxComponent* boxCollider;

private:
	UFUNCTION()
	void OnPlayerDetected(APawn* pawn);

	UFUNCTION()
	void DeathSequence();

	APawn* currentTarget;

	// Animation flags.
	bool bScreamAnimPlaying;
	bool bChargeAnimPlaying;
	bool bMoveAnimiPlaying;
	bool bDeathAnimPlaying;

	// Death flag.
	bool bDead;

	float prevHealth;
	
};
