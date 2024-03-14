// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Indicator.generated.h"

UCLASS()
class SHADOWRUNNER_API AIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Mesh component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
		UStaticMeshComponent* pickupMesh;
	// Scene component.
	/*PROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
		USceneComponent* sceneComponent;*/
};
