// Fill out your copyright notice in the Description page of Project Settings.

#include "ShadowMonsterSpwan.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShadowRunnerCharacter.h"
#include "UObject/Object.h"
#include "GameFramework/Controller.h"
#include "Engine/EngineTypes.h"

int arenaDie = 0;
extern int magicMax;
extern float volumeControlEnemy;
// Sets default values
AShadowMonsterSpwan::AShadowMonsterSpwan()
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
	spawnTime2 = 2.f;
	spawnTime = 2.f;
	isFinalRoom = false;
	winCheck = false;
	finalSpawnDistance = 13250.f;
	firstWin = false;

	static ConstructorHelpers::FObjectFinder<UClass> bpClassFinder(TEXT("/Game/BluePrints/Enemy/Melee/Mutant/EnemyCharacter_BP.EnemyCharacter_BP_C"));
	WhatToSpawn = bpClassFinder.Object;

	//const FString SpawnName = FString::Printf(TEXT("Blueprint'/Game/BluePrints/Enemy/Melee/Mutant/EnemyCharacter_BP.EnemyCharacter_BP'"));
	//WhatToSpawn = LoadObject<UBlueprint>(nullptr, *SpawnName)->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> PS(TEXT("NiagaraSystem'/Game/BluePrints/particle/ability_Niagara_2.ability_Niagara_2'"));
	Particle_ability = PS.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> PS2(TEXT("SoundWave'/Game/Assets/Music/_Wave_Spawn_Alarms__Buzzer__Evacuation__Malfunction__Large__Danger_01_SND7473_1._Wave_Spawn_Alarms__Buzzer__Evacuation__Malfunction__Large__Danger_01_SND7473_1'"));
	SpawnSound = PS2.Object;
}

FVector AShadowMonsterSpwan::GetRandomPointInVolume()
{
	FVector location = { 0.f,0.f,0.f };

	//if (first == true)
	//{
	//}
	//else
	//{
		//location = NavArea->GetRandomReachablePointInRadius(this, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), spawnRange);
	//}

	for (int i = 0; i < maxSpawnCount; i++)
	{
		location = NavArea->GetRandomReachablePointInRadius(this, GetActorLocation(), spawnRange);

		if (location.Y < this->GetActorLocation().Y)
		{
			location.Y = this->GetActorLocation().Y;
		}

		susu.push_back(location);
	}

	return location;
}

void AShadowMonsterSpwan::GetRandomPointInVolume2()
{
	finalLocation.push_back({ 18020.0f, -2370.0f, 115.398338f });
	finalLocation.push_back({ 18010.0, -1530.0f, 115.398338f });
	finalLocation.push_back({ 18320.0, -680.0f, 115.398338f });
	finalLocation.push_back({ 17160.0, -2460.0f, 115.398338f });
	finalLocation.push_back({ 17160.0, -1580.0f, 115.398338f });
	finalLocation.push_back({ 17160.0, -550.0f, 115.398338f });
	finalLocation.push_back({ 16460.0, -2040.0f, 115.398338f });
	finalLocation.push_back({ 16380.0, -1650.0f, 115.398338f });
	finalLocation.push_back({ 16260.0, -1130.0f, 115.398338f });
}

// Called when the game starts or when spawned
void AShadowMonsterSpwan::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	GetRandomPointInVolume2();
	GetRandomPointInVolume();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("wow sibal")));

	//CharacterStartPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
}

// Called every frame
void AShadowMonsterSpwan::Tick(float DeltaTime)
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

			float firstTime = DeltaTime;

			if ((abs(playerX - spawnerX) <= firstSpawnDistance) && (abs(playerY - spawnerY) <= firstSpawnDistance))
			{
				if (SpawnSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation(), volumeControlEnemy * 0.6f);
				}


					//if (Particle_ability)
					//{
					//	particleComp3 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle_ability, susu[1]);
					//}

					//if (Particle_ability)
					//{
					//	particleComp4 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle_ability, susu[2]);
					//}

					//GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]() {this->SpawnObject(); for (int i = 0; i < maxSpawnCount; i++) { particleComp[i]->DestroyInstance(); }}), spawnTime, false);

					this->SpawnObject();

				//for (int i = 0; i < susu.size(); i++) 
				//{ 
				//	 
				//}

				//	for (int i = 0; i < maxSpawnCount; i++)
				//	{
				//		this->SpawnObject();
				//	}

				once = false;
				first = false;

				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("y cor: %d"), first));

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
						//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d subal"), countDeath));
					}
				}
			}
		}


		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d subal"), subal));

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
					UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation(), volumeControlEnemy*0.6f);
				}

				this->SpawnObject();

				AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
				shadowRunnerHUD->UpdateWaveSystem(countingWave);

				once = false;
			}
		}
	}
	else
	{

		if (first == true)
		{
			countDeath = 0;

			float spawnerX = this->GetActorLocation().X;
			float spawnerY = this->GetActorLocation().Y;

			float playerX = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().X;
			float playerY = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Y;

			float firstTime = DeltaTime;

			if (playerX >= finalSpawnDistance && playerY >= 40.f)
			{
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFirstDoorActor::StaticClass(), FoundActors);
				AFirstDoorActor* theDoor = nullptr;
				AFirstDoorActor* theFinalDoor = nullptr;
				for (int i = 0; i < FoundActors.Num(); ++i)
				{
					AFirstDoorActor* door_i = Cast<AFirstDoorActor>(FoundActors[i]);
					if (door_i)
					{
						if (door_i->doorId == 1116) // change the number here.
						{
							theDoor = door_i;
							break;
						}

						if (door_i->doorId == 309) // change the number here.
						{
							theFinalDoor = door_i;
							break;
						}
					}
				}

				if (theDoor)
				{
					//theDoor->ToggleDoor(FVector(1.f, 0.f, 0.f));
					theDoor->doorState = AFirstDoorActor::DoorState::CLOSING;
					theDoor->keyId = 777;
				}
				if (theFinalDoor)
				{
					//theFinalDoor->doorState = AFirstDoorActor::DoorState::CLOSING;
					theFinalDoor->keyId = 879;
				}


				if (SpawnSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation(), volumeControlEnemy * 0.6f);
				}


				this->SpawnObject2();

				once = false;
				first = false;

				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("y cor: %d"), first));

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
						arenaDie++;

						checkEenmyHealthOnce[i] = true;
					}
				}
			}
		}

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d die"), arenaDie));

		if (arenaDie == 9)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("clear!!")));

			firstWin = false;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("kill all enemy!!22")));

			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFirstDoorActor::StaticClass(), FoundActors);
			AFirstDoorActor* theDoor = nullptr;

			for (int i = 0; i < FoundActors.Num(); ++i)
			{
				AFirstDoorActor* door_i = Cast<AFirstDoorActor>(FoundActors[i]);
				if (door_i)
				{
					if (door_i->doorId == 309) // change the number here.
					{
						theDoor = door_i;
						break;
					}
				}
			}

			if (firstWin == false)
			{
				winCheck = true;
				firstWin = true;
				arenaDie = 0;
				{
					if (theDoor)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("kill all enemy!!")));
						theDoor->keyId = 0;
						theDoor->doorState = AFirstDoorActor::DoorState::OPENING;
						//theDoor->ToggleDoor(FVector(1.f, 0.f, 0.f));
					}
				}
				//ChangeLevel();
			}
		}
	}
}

void AShadowMonsterSpwan::SpawnObject()
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

			AEnemyCharacter* SpawnMon = GetWorld()->SpawnActor<AEnemyCharacter>(WhatToSpawn, susu[i], SpawnRotation, SpawnParams);

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("X %d"), SpawnMon->GetActorLocation().X));
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Y %d"), SpawnMon->GetActorLocation().Y));
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Z %d"), SpawnMon->GetActorLocation().Z));

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

void AShadowMonsterSpwan::SpawnObject2()
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

		AEnemyCharacter* SpawnMon = GetWorld()->SpawnActor<AEnemyCharacter>(WhatToSpawn, finalLocation[i], SpawnRotation, SpawnParams);

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

void AShadowMonsterSpwan::ChangeLevel()
{
	UWorld* TheWorld = GetWorld();

	FString CurrentLevel = TheWorld->GetMapName();

	if (CurrentLevel == "Prototype_Level")
	{
		UGameplayStatics::OpenLevel(GetWorld(), "GameEnding");
	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Prototype_Level");
	}
}