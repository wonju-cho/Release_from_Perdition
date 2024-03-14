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
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include <vector>
#include "ShadowMonsterSpwan.generated.h"

UCLASS()
class SHADOWRUNNER_API AShadowMonsterSpwan : public AActor
{
	GENERATED_BODY()

public:
	AShadowMonsterSpwan();
	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector GetRandomPointInVolume();

	void GetRandomPointInVolume2();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int waves;

	void ChangeLevel();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* DoorOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)

		USoundBase* DoorLockedSound;
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
		float finalSpawnDistance;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		float spawnTime;


	UPROPERTY(EditAnywhere, Category = "Spawning")
		float spawnTime2;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		bool isFinalRoom;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		bool winCheck;

	class UStaticMeshComponent* addMesh;


	UPROPERTY(EditAnywhere, Category = "Spawning")
		UNiagaraSystem* Particle_ability;

	UNiagaraComponent* particleComp[20];
	UNiagaraComponent* particleComp1;
	UNiagaraComponent* particleComp3;
	UNiagaraComponent* particleComp4;

	UNiagaraComponent* particleComp2;

private:
	virtual void Tick(float DeltaTime) override;

	class UNavigationSystemV1* NavArea;

	TArray<class AEnemyCharacter*> SpawnObjects;

	void SpawnObject();
	void SpawnObject2();

	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);


	bool once = true;
	bool first;
	bool firstWin;
	int countDeath;

	std::vector<AEnemyCharacter*> checkEenmyHealth;
	std::vector<bool> checkEenmyHealthOnce;

	std::vector<FVector> finalLocation;

	std::vector<FVector> susu;

	FTimerHandle WaitHandle;
	FTimerHandle WaitHandle2;
};
