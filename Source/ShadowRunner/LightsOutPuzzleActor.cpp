// Fill out your copyright notice in the Description page of Project Settings.


#include "LightsOutPuzzleActor.h"
#include "Components/ArrowComponent.h"
#include "DoubleJumpPickupActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALightsOutPuzzleActor::ALightsOutPuzzleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// Arrow component.
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrow->SetupAttachment(RootComponent);

	// Meshes.
	// 1 2 3
	// 4 5 6
	// 7 8 9
	cube1Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube1Mesh"));
	cube1Mesh->SetupAttachment(RootComponent);
	cube2Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube2Mesh"));
	cube2Mesh->SetupAttachment(RootComponent);
	cube3Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube3Mesh"));
	cube3Mesh->SetupAttachment(RootComponent);
	cube4Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube4Mesh"));
	cube4Mesh->SetupAttachment(RootComponent);
	cube5Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube5Mesh"));
	cube5Mesh->SetupAttachment(RootComponent);
	cube6Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube6Mesh"));
	cube6Mesh->SetupAttachment(RootComponent);
	cube7Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube7Mesh"));
	cube7Mesh->SetupAttachment(RootComponent);
	cube8Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube8Mesh"));
	cube8Mesh->SetupAttachment(RootComponent);
	cube9Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube9Mesh"));
	cube9Mesh->SetupAttachment(RootComponent);

	// Initialize cubes array.
	cubes.Add(cube1Mesh);
	cubes.Add(cube2Mesh);
	cubes.Add(cube3Mesh);
	cubes.Add(cube4Mesh);
	cubes.Add(cube5Mesh);
	cubes.Add(cube6Mesh);
	cubes.Add(cube7Mesh);
	cubes.Add(cube8Mesh);
	cubes.Add(cube9Mesh);

	// Initialize boolean flags.
	PuzzleIsSolved = false;

	// Stored patterns.
	// 1 2 3
	// 4 5 6
	// 7 8 9
	patterns = { 
		{1, 2, 4, 5},
		{1, 2, 4, 6, 8, 9},
		{1, 3, 4, 7},
		{1, 3, 6},
		{1, 3, 4, 5, 6, 8},
		{1, 4, 6, 8},
		{1, 4, 5, 7, 8},
		{1, 5, 6, 8, 9},
		{2, 3, 5, 6, 7, 8, 9},
		{2, 4, 5, 7, 9},
		{2, 3},
		{2, 9},
		{2, 3, 4, 5, 6, 8},
		{2, 4, 6, 8, 9},
		{2, 4, 8},
		{3, 6, 9},
		{4, 6, 7, 8, 9},
		{5, 6, 7, 8, 9},
		{6, 7, 9},
		{6, 9}
	};
}

// Called when the game starts or when spawned
void ALightsOutPuzzleActor::BeginPlay()
{
	Super::BeginPlay();

	SetupPuzzle();
}

#if 0 // Not needed now.
// Called every frame
void ALightsOutPuzzleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#endif

void ALightsOutPuzzleActor::ToggleCubes(UStaticMeshComponent* target)
{
	if (!PuzzleIsSolved)
	{
		// Play toggle sound effect
		if (cubeToggleSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, cubeToggleSound, GetActorLocation(), GetActorRotation(), 0.75);
		}

		// Toggle the target cube.
		ToggleMesh(target);

		// Toggle adjacent cubes.
		// 1 2 3
		// 4 5 6
		// 7 8 9
		if (target->GetName() == FString("Cube1Mesh"))
		{
			ToggleMesh(cube2Mesh);
			ToggleMesh(cube4Mesh);
		}
		else if (target->GetName() == FString("Cube2Mesh"))
		{
			ToggleMesh(cube1Mesh);
			ToggleMesh(cube3Mesh);
			ToggleMesh(cube5Mesh);
		}
		else if (target->GetName() == FString("Cube3Mesh"))
		{
			ToggleMesh(cube2Mesh);
			ToggleMesh(cube6Mesh);
		}
		else if (target->GetName() == FString("Cube4Mesh"))
		{
			ToggleMesh(cube1Mesh);
			ToggleMesh(cube5Mesh);
			ToggleMesh(cube7Mesh);
		}
		else if (target->GetName() == FString("Cube5Mesh"))
		{
			ToggleMesh(cube2Mesh);
			ToggleMesh(cube4Mesh);
			ToggleMesh(cube6Mesh);
			ToggleMesh(cube8Mesh);
		}
		else if (target->GetName() == FString("Cube6Mesh"))
		{
			ToggleMesh(cube3Mesh);
			ToggleMesh(cube5Mesh);
			ToggleMesh(cube9Mesh);
		}
		else if (target->GetName() == FString("Cube7Mesh"))
		{
			ToggleMesh(cube4Mesh);
			ToggleMesh(cube8Mesh);
		}
		else if (target->GetName() == FString("Cube8Mesh"))
		{
			ToggleMesh(cube5Mesh);
			ToggleMesh(cube7Mesh);
			ToggleMesh(cube9Mesh);
		}
		else if (target->GetName() == FString("Cube9Mesh"))
		{
			ToggleMesh(cube6Mesh);
			ToggleMesh(cube8Mesh);
		}

		// Check if the puzzle is solved.
		CheckPuzzle();
	}
}

void ALightsOutPuzzleActor::ToggleMesh(UStaticMeshComponent* target)
{
	if (target->GetMaterial(0) == offMaterial)
	{
		if (onMaterial)
		{
			target->SetMaterial(0, onMaterial);
		}
	}
	else if (target->GetMaterial(0) == onMaterial)
	{
		if (offMaterial)
		{
			target->SetMaterial(0, offMaterial);
		}
	}
}

void ALightsOutPuzzleActor::CheckPuzzle()
{
	for (int i = 0; i < 9; ++i)
	{
		if (cubes[i]->GetMaterial(0) == onMaterial)
		{
			return;
		}
	}

	// All cubes are off.
	PuzzleIsSolved = true;

	FActorSpawnParameters spawnParams;
	GetWorld()->SpawnActor<ADoubleJumpPickupActor>(pickup, this->GetActorLocation() + arrow->GetForwardVector() * 80.0f, this->GetActorRotation(), spawnParams);

	if (puzzleSolvedSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, puzzleSolvedSound, GetActorLocation(), GetActorRotation(), 0.75);
	}

	// Test, should be removed later.
	//SetupPuzzle();
}

void ALightsOutPuzzleActor::SetupPuzzle()
{
	// Random number for pattern select.
	int32 n = FMath::RandRange(0, patterns.Num() - 1);

	for (int i = 0; i < patterns[n].Num(); ++i)
	{
		cubes[patterns[n][i] - 1]->SetMaterial(0, onMaterial);
	}
}

void ALightsOutPuzzleActor::TurnOffCube(UStaticMeshComponent* target)
{
	if (offMaterial)
	{
		target->SetMaterial(0, offMaterial);
	}

	// Check if the puzzle is solved.
	CheckPuzzle();
}