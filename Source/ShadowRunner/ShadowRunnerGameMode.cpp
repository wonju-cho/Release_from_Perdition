// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShadowRunnerGameMode.h"
#include "ShadowRunnerHUD.h"
#include "ShadowRunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "KeyPickupActor.h"

AShadowRunnerGameMode::AShadowRunnerGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AShadowRunnerHUD::StaticClass();
}

//void AShadowRunnerGameMode::Respawn(AController* Controller)
//{
//	if(Role == ROLE_Authority)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("In Respawn"));
//	}
//}

void AShadowRunnerGameMode::CheckBlueCube(FVector location, FRotator rotation)
{
	if (counter == 3)
	{
		FActorSpawnParameters spawnParams;

		// Spawn dash pickup.
		//dashActorRef = GetWorld()->SpawnActor<AActor>(dashItemClone, location, rotation, spawnParams);
		if (keyItemClone)
		{
			keyActorRef = GetWorld()->SpawnActor<AActor>(keyItemClone, location, rotation, spawnParams);
			AKeyPickupActor* key = Cast<AKeyPickupActor>(keyActorRef);
			if (key)
			{
				key->targetDoorId = 777;
			}
		}
	}

#if 0
	if (counter == 6)
	{
		FActorSpawnParameters spawnParams;

		// Spawn character.
		dashActorRef = GetWorld()->SpawnActor<AActor>(dashItemClone, location, rotation, spawnParams);
	}
#endif
}
