// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolPath.h"
#include "Animation/AnimMontage.h"
#include "EnemyCharacter.generated.h"

class UHealthComponent;

UCLASS()
class SHADOWRUNNER_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	float GetEnemyHealth();

	void SetEnemyHealth(float newHeatlh);

	float GetEnemyMaxHealth();

	UFUNCTION(BlueprintCallable)
		void TakeDamage(float damage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float EnemyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float EnemyMaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		bool HasTakenDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		bool IsDead;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APatrolPath* GetPatrolPath();

	void MeleeAttack();

private:

	UPROPERTY(EditAnywhere)
	class UBoxComponent* AttackHitBox;

	UFUNCTION()
	void OnDealDamageToPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool damageTimerFlag;

	float deathCount;
public:

	UAnimMontage* GetMontage() const;

	UPROPERTY(EditAnywhere)
		float DamageAmount = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* DyingSound;
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* OnHitSound;

	struct FTimerHandle DamageTimerHandle;

	FTimerDelegate DamageTimerDelegate;

	void HitTimerCallback();
	
	float soundVolume = 0.5f;

	UFUNCTION(BlueprintCallable)
		float GetVolumes();

	UFUNCTION(BlueprintCallable)
		void SetVolumes(float s);

private:

	float MontageLength;

	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Montage;

	// Shadow clone variables.
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> buffClone;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> buffClone2;

	AActor* buffActorRef; // Clone reference for destroying the clone.

	AActor* buffActorRef2; // Clone reference for destroying the clone.

	int randomNum;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* hitMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* baseMaterial;

	// Timer handle for changing material.
	struct FTimerHandle hitTimerHandle;

	float hitDisplayTime;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	//	USoundBase* DistractionSound;

	//void OnDistract();

	float SpawnCount = 0;
	bool buffSpawned;
};
