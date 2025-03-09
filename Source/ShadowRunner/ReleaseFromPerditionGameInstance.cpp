// Fill out your copyright notice in the Description page of Project Settings.

#include "ReleaseFromPerditionGameInstance.h"
#include "UTextureManager.h"


void UReleaseFromPerditionGameInstance::Shutdown ()
{
	Super::Shutdown();

	if(UUTextureManager::Get())
	{
		UUTextureManager::Get()->ShutDown();
	}

	UE_LOG(LogTemp, Warning, TEXT("Game Shutdown is detected: UGameInstance::Shutdown() is called!"));
}