// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AImagician\AIMagic.h"
#include "GameFramework/Actor.h"
#include "ShadowRunnerHUD.h"
#include "ShadowRunnerCharacter.h"
#include "NavigationSystem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include <vector>
#include "ShadowMagicSpawn.generated.h"

UCLASS()
class SHADOWRUNNER_API AShadowMagicSpawn : public AActor
{
	GENERATED_BODY()

public:
	AShadowMagicSpawn();

	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector GetRandomPointInVolume();

	void GetRandomPointInVolume2();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int waves;

protected:
	// Called when the game starts or when spawned
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
		float finalSpawnDistance;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		bool isFinalRoom;

	class UStaticMeshComponent* addMesh;


	UPROPERTY(EditAnywhere, Category = "Spawning")
		UNiagaraSystem* Particle_ability;

	UNiagaraComponent* particleComp[20];

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UNavigationSystemV1* NavArea;

	TArray<class AAIMagic*> SpawnObjects;

	void SpawnObject();
	void SpawnObject2();

	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	bool once = true;
	bool first;
	int countDeath;

	std::vector<AAIMagic*> checkEenmyHealth;
	std::vector<bool> checkEenmyHealthOnce;

	std::vector<FVector> finalLocation;

	std::vector<FVector> susu;
	
	FTimerHandle WaitHandle;
	FTimerHandle WaitHandle2;
};
