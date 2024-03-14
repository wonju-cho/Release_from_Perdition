// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstDoorActor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacter.h"
#include "ShadowRunnerHUD.h"
#include "ShadowRunnerCharacter.h"
#include "NavigationSystem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FlyingEnemyCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include <vector>
#include "ShadowFlyEnemySpawn.generated.h"

UCLASS()
class SHADOWRUNNER_API AShadowFlyEnemySpawn : public AActor
{
	GENERATED_BODY()

public:
	AShadowFlyEnemySpawn();
	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector GetRandomPointInVolume();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int waves;
protected:

	//FVector CharacterStartPosition;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class APawn> WhatToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		int maxSpawnCount;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		int maxWave;

	int countingWave;
	int currWave;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		float spawnRange;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		float firstSpawnDistance;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		float spawnTime;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		float spawnTime2;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		bool isFinalRoom;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		float finalSpawnDistance;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		UNiagaraSystem* Particle_ability;

	UNiagaraComponent* particleComp[20];

	class UStaticMeshComponent* addMesh;
private:
	virtual void Tick(float DeltaTime) override;

	class UNavigationSystemV1* NavArea;

	TArray<class AEnemyCharacter*> SpawnObjects;

	void SpawnObject();

	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	bool once = true;
	bool first;
	int countDeath;

	std::vector<AFlyingEnemyCharacter*> checkEenmyHealth;
	std::vector<bool> checkEenmyHealthOnce;
	std::vector<FVector> spawnLocations;

	std::vector<FVector> susu;

	FTimerHandle WaitHandle;
	FTimerHandle WaitHandle2;
};
