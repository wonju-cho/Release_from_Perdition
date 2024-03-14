// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interactable_Cube1.h"
#include "ShadowRunnerGameMode.generated.h"

UCLASS(minimalapi)
class AShadowRunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> itemClone;
	AActor* spawnedActorRef;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> dashItemClone;
	AActor* dashActorRef;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> keyItemClone;

	AActor* keyActorRef;

	
public:
	AShadowRunnerGameMode();
	UPROPERTY()
	int counter;

	// Gameover screen survived time.
	UPROPERTY()
	FString hourString;

	UPROPERTY()
	FString minString;

	UPROPERTY()
	FString secString;

public:
	//void Respawn(AController* Controller);
	void CheckBlueCube(FVector location, FRotator rotation);

protected:
	//void SpawnItem(FVector location);
};



