// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultEnemy.generated.h"

UCLASS()
class SHADOWRUNNER_API ADefaultEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADefaultEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void TakeDamage(float damage);

	UFUNCTION(BlueprintCallable)
		void Dead();

	UFUNCTION(BlueprintCallable)
		void Respawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float EnemyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		FVector SpawnPosition;

};
