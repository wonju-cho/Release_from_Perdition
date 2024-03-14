// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowMagicSpawn.h"
#include "Engine/EngineTypes.h"
//#include "RenderActor.h"
//#include "UObject/UObjectGlobals.h"

extern int arenaDie;
int magicMax = 0;
extern float volumeControlEnemy;

// Sets default values
AShadowMagicSpawn::AShadowMagicSpawn()
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
	isFinalRoom = false;
	finalSpawnDistance = 13250.f;

	 //(TEXT("Material'/Game/Assets/Materials/BlueCube'"));

	static ConstructorHelpers::FObjectFinder<UClass> bpClassFinder(TEXT("/Game/BluePrints/AIMagician/AIMagic_BP.AIMagic_BP_C"));
	WhatToSpawn = bpClassFinder.Object;

	//MyBlueprint = LoadObject<UBlueprint>(nullptr, TEXT("Blueprint'/Game/BluePrints/AIMagician/AIMagic_BP.AIMagic_BP'"));
	//WhatToSpawn = MyBlueprint->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> PS(TEXT("NiagaraSystem'/Game/BluePrints/particle/ability_Niagara_2.ability_Niagara_2'"));
	Particle_ability = PS.Object;
	//
	static ConstructorHelpers::FObjectFinder<USoundBase> PS2(TEXT("SoundWave'/Game/Assets/Music/_Wave_Spawn_Alarms__Buzzer__Evacuation__Malfunction__Large__Danger_01_SND7473_1._Wave_Spawn_Alarms__Buzzer__Evacuation__Malfunction__Large__Danger_01_SND7473_1'"));
	SpawnSound = PS2.Object;

	//const FString SpawnName = FString::Printf(TEXT("/Game/BluePrints/AIMagician/AIMagic_BP"));
	//SpawnSound = LoadObject<UBlueprint>(nullptr, *SpawnName)->GeneratedClass;

	//Particle_ability = CreateDefaultSubobject<UNiagaraSystem>(TEXT("MyPSC"));

	//const FString PrticleName = FString::Printf(TEXT("NiagaraSystem'/Game/BluePrints/particle/ability_Niagara_2.ability_Niagara_2'"));
	//Particle_ability->SetTemplate()
	//Particle_ability = LoadObject<UBlueprint>(nullptr, *PrticleName)->GeneratedClass;
	//WhatToSpawn = LoadObject<UBlueprint>(nullptr, TEXT("/Content/BluePrints/AIMagician/AIMagic_BP"))->GeneratedClass;
	//FOutputDeviceNull sibal;
	//WhatToSpawn->CallFunctionByNameWithArguments(*name, sibal, NULL, true);
	//WhatToSpawn = LoadClass<AAIMagic>(NULL, TEXT("/Content/BluePrints/AIMagic_BP"), NULL, LOAD_None, NULL);
	//WhatToSpawn = Cast<AAIMagic>(StaticLoadObject(AAIMagic::StaticClass(), NULL, *name));
	//static ConstructorHelpers::FObjectFinder<UClass> PawnObject(*name);
	//WhatToSpawn = PawnObject.Object; // TSubclassOf<class APAwn> PawnClass; is declared in my class' header.
}

FVector AShadowMagicSpawn::GetRandomPointInVolume()
{
	FVector location = { 0.f,0.f,0.f };

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

void AShadowMagicSpawn::GetRandomPointInVolume2()
{
	finalLocation.push_back({ 15610.0f,1090.0f, 115.398338f });
	finalLocation.push_back({ 17160.0, 1090.0f, 115.398338f });
}

// Called when the game starts or when spawned
void AShadowMagicSpawn::BeginPlay()
{
	Super::BeginPlay();
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	GetRandomPointInVolume2();
	GetRandomPointInVolume();
}

// Called every frame
void AShadowMagicSpawn::Tick(float DeltaTime)
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
					if (checkEenmyHealth[i]->GetHealth() <= 0 && checkEenmyHealthOnce[i] == false)
					{
						countDeath++;
						checkEenmyHealthOnce[i] = true;
					}
				}
			}
		}

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
	else
	{
		magicMax = maxSpawnCount;

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
				if (SpawnSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation(), volumeControlEnemy * 0.6f);
				}

				this->SpawnObject2();

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
					if (checkEenmyHealth[i]->GetHealth() <= 0 && checkEenmyHealthOnce[i] == false)
					{
						countDeath++;
						arenaDie++;

						checkEenmyHealthOnce[i] = true;
					}
				}
			}
		}
	}
}

void AShadowMagicSpawn::SpawnObject()
{
	if (susu.size() > 0)
	{
		for (int i = 0; i < maxSpawnCount; i++)
		{

			FActorSpawnParameters    SpawnParams;
			SpawnParams.Owner = this;

			//FVector SpawnLocation = GetRandomPointInVolume();

			FRotator SpawnRotation;

			SpawnRotation.Pitch = 0.0f;

			SpawnRotation.Yaw = 0.0f;

			SpawnRotation.Roll = 0.0f;

			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AAIMagic* SpawnMon = GetWorld()->SpawnActor<AAIMagic>(WhatToSpawn, susu[i], SpawnRotation, SpawnParams);

			if (SpawnMon != NULL)
			{
				SpawnMon->SpawnDefaultController();
			}

			checkEenmyHealth.push_back(SpawnMon);
			checkEenmyHealthOnce.push_back(false);
		}
	}
	
}

void AShadowMagicSpawn::SpawnObject2()
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

		AAIMagic* SpawnMon = GetWorld()->SpawnActor<AAIMagic>(WhatToSpawn, finalLocation[i], SpawnRotation, SpawnParams);

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