// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "ShadowRunner/MagicProjectile.h"
#include <ShadowRunner/AImagician/MainAnimInstance.h>
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "ShadowRunner/Indicator.h"
#include "AIMagic.generated.h"

UCLASS()
class SHADOWRUNNER_API AAIMagic : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	
	FVector SpawnLocation;
	UFUNCTION(BlueprintCallable, Category = "Mesh")
		FVector getLocation();

	AAIMagic();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AMagicProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere)
		float health;

	UPROPERTY(EditAnywhere)
		bool OnCylinder = false;

	bool checkCylinder = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	float prevHealth;

	// Pickup drop.
	UPROPERTY(EditDefaultsOnly, Category = Drop)
		TSubclassOf<class AHealthPickupActor> healthDropClass;

	UPROPERTY(EditDefaultsOnly, Category = Drop)
		TSubclassOf<class AAmmoPickupActor> ammoDropClass;
	float GetHealth();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MagicAttackPlay();
	void Attack();
	void ShadowAttack();

	//struct FTimerHandle TimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "particle indication")
		UNiagaraSystem* Particle_ability;

	UNiagaraComponent* particleComp;

	void particlePlay();


	UAnimMontage* GetMontage() const;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	struct FTimerHandle AttackTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* DyingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UParticleSystem* MuzzleParticles;

	bool IsDead;
	bool Buff = false;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* hitMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* baseMaterial;

	// Timer handle for changing material.
	struct FTimerHandle hitTimerHandle;

	float hitDisplayTime;

	UFUNCTION()
	void HitTimerCallback();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* montage;

	FTimerHandle AttackHandle;

	FVector playerLocations;

	float deathCount = 0;
};
