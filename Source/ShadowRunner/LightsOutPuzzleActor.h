// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightsOutPuzzleActor.generated.h"

UCLASS()
class SHADOWRUNNER_API ALightsOutPuzzleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightsOutPuzzleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Arrow component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UArrowComponent* arrow;

	// Meshes.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeMeshes")
	class UStaticMeshComponent* cube1Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeMeshes")
	class UStaticMeshComponent* cube2Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeMeshes")
	class UStaticMeshComponent* cube3Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeMeshes")
	class UStaticMeshComponent* cube4Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeMeshes")
	class UStaticMeshComponent* cube5Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeMeshes")
	class UStaticMeshComponent* cube6Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeMeshes")
	class UStaticMeshComponent* cube7Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeMeshes")
	class UStaticMeshComponent* cube8Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubeMeshes")
	class UStaticMeshComponent* cube9Mesh;

	// Materials.
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* onMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* offMaterial;

	// Pickup drop when puzzle solved.
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> pickup;

	// Boolean flags.
	bool PuzzleIsSolved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	USoundBase* cubeToggleSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	USoundBase* puzzleSolvedSound;

	// For looping.
	TArray<class UStaticMeshComponent*> cubes;

	// Functions.
	void ToggleCubes(UStaticMeshComponent* target);

	void TurnOffCube(UStaticMeshComponent* target);

private:
	// Stored patterns.
	TArray<TArray<int32>> patterns;

	void ToggleMesh(UStaticMeshComponent* target);

	void CheckPuzzle();

	void SetupPuzzle();
};
