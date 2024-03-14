// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowFlyEnemySpawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShadowRunnerCharacter.h"
#include "UObject/Object.h"
#include "GameFramework/Controller.h"
#include "Engine/EngineTypes.h"

extern int arenaDie;
extern float volumeControlEnemy;

// Sets default values
AShadowFlyEnemySpawn::AShadowFlyEnemySpawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	maxSpawnCount = 1;
	spawnRange = 500.f;
	firstSpawnDistance = 500.f;
	maxWave = 3;
	countingWave = 0;
	currWave = maxWave;
	addMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("<SPAWNER>"));
	addMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	waves = 0;
	first = true;
	countDeath = 0;
	spawnTime = 2.f;
	spawnTime2 = 2.f;
	finalSpawnDistance = 13250.f;
	isFinalRoom = false; 

	static ConstructorHelpers::FObjectFinder<UClass> bpClassFinder(TEXT("/Game/BluePrints/Enemy/FlyingEnemy/FlyingEnemyCharacter_BP.FlyingEnemyCharacterVer2_BP_C"));
	WhatToSpawn = bpClassFinder.Object;

	//const FString SpawnName = FString::Printf(TEXT("Blueprint'/Game/BluePrints/Enemy/FlyingEnemy/FlyingEnemyCharacter_BP.FlyingEnemyCharacterVer2_BP'"));
	//WhatToSpawn = LoadObject<UBlueprint>(nullptr, *SpawnName)->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> PS(TEXT("NiagaraSystem'/Game/BluePrints/particle/ability_Niagara_2.ability_Niagara_2'"));
	Particle_ability = PS.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> PS2(TEXT("SoundWave'/Game/Assets/Music/_Wave_Spawn_Alarms__Buzzer__Evacuation__Malfunction__Large__Danger_01_SND7473_1._Wave_Spawn_Alarms__Buzzer__Evacuation__Malfunction__Large__Danger_01_SND7473_1'"));
	SpawnSound = PS2.Object;
}

FVector AShadowFlyEnemySpawn::GetRandomPointInVolume()
{
	FVector location = { 0.f,0.f,0.f };

	for (int i = 0; i < maxSpawnCount; i++)
	{
		location = NavArea->GetRandomReachablePointInRadius(this, GetActorLocation(), spawnRange);
		location.Y = this->GetActorLocation().Y;
		location.Z = this->GetActorLocation().Z;

		susu.push_back(location);
	}

	return location;
}

// Called when the game starts or when spawned
void AShadowFlyEnemySpawn::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	GetRandomPointInVolume();
	//for (int i = 0; i < maxSpawnCount; i++)
	//{
	//	spawnLocations[i] = GetRandomPointInVolume();
	//}

}

// Called every frame
void AShadowFlyEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isFinalRoom == false)
	{
		if (first == true)
		{
			countDeath = 0;

			float spawnerX = this->GetActorLocation().X;
			float spawnerY = this->GetActorLocation().Y;

			float playerX = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().X;
			float playerY = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Y;

			if ((abs(playerX - spawnerX) <= firstSpawnDistance) && (abs(playerY - spawnerY) <= firstSpawnDistance))
			{

				if (SpawnSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation(), volumeControlEnemy * 0.6f);
				}

				this->SpawnObject();

				once = false;
				first = false;

				countingWave++;
				AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
				shadowRunnerHUD->UpdateWaveSystem(countingWave);
			}
		}

		if (checkEenmyHealth.empty() != true)
		{
			if (countDeath != maxSpawnCount)
			{
				for (int i = 0; i < checkEenmyHealth.size(); i++)
				{
					if (checkEenmyHealth[i]->GetEnemyHealth() <= 0 && checkEenmyHealthOnce[i] == false)
					{
						countDeath++;
						checkEenmyHealthOnce[i] = true;
					}
				}
			}
		}

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("countDeath %d"), countDeath));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("countingWave %d"), countingWave));


		if (countDeath == maxSpawnCount  && countingWave < maxWave)
		{
			once = true;
			countDeath = 0;
			countingWave++;

			checkEenmyHealth.clear();
			checkEenmyHealthOnce.clear();

			if (once == true)
			{
				if (SpawnSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation(), volumeControlEnemy * 0.6f);
				}

				this->SpawnObject();

				AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
				shadowRunnerHUD->UpdateWaveSystem(countingWave);

				once = false;
			}
		}
	}
}

void AShadowFlyEnemySpawn::SpawnObject()
{
	if (susu.size() > 0)
	{
		for (int i = 0; i < maxSpawnCount; i++)
		{

			FActorSpawnParameters    SpawnParams;
			SpawnParams.Owner = this;
			//SpawnParams.Instigator = Instigator;

			//FVector SpawnLocation = GetRandomPointInVolume();

			FRotator SpawnRotation;


			SpawnRotation.Pitch = 0.0f;

			SpawnRotation.Yaw = 0.0f;

			SpawnRotation.Roll = 0.0f;

			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			//for(int i = 0; i < )
			AFlyingEnemyCharacter* SpawnMon = GetWorld()->SpawnActor<AFlyingEnemyCharacter>(WhatToSpawn, susu[i], SpawnRotation, SpawnParams);

			if (SpawnMon != NULL)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("you %d"), j));

				SpawnMon->SpawnDefaultController();
				//j++;
			}

			checkEenmyHealth.push_back(SpawnMon);
			checkEenmyHealthOnce.push_back(false);
		}
	}
}



