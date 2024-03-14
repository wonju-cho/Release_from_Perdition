// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Master_Interactable.h"
#include "ShadowRunnerGameMode.h"
#include "Interactable_Cube1.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API AInteractable_Cube1 : public AMaster_Interactable
{
	GENERATED_BODY()
	
public:
	virtual void Interact() override;

	UPROPERTY()
	bool State;

	UPROPERTY()
	class AShadowRunnerGameMode* gameMode;

	UPROPERTY(EditAnywhere, Category = "Spawn location")
	FVector spawnLocation;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StaticMeshComponents) UStaticMeshComponent* cube;

	UMaterialInterface* mat;

	//static ConstructorHelpers::FObjectFinder <UMaterialInterface>Material_Blue(TEXT("Material'/Game/Assets/Materials/BlueCube'"));

	AInteractable_Cube1();
};
