// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShadowCloneCharacter.generated.h"

UCLASS()
class SHADOWRUNNER_API AShadowCloneCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShadowCloneCharacter();

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

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	float health;

	UFUNCTION()
	void SpawnProjectile();

private:
	UFUNCTION()
	void OnEnemyDetected(APawn* pawn);

	APawn* currentTarget;
};
