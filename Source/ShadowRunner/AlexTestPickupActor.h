// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"

#include "AlexTestPickupActor.generated.h"

UCLASS()
class SHADOWRUNNER_API AAlexTestPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlexTestPickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Mesh component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	UStaticMeshComponent* pickupMesh;

	// Rotation rate.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	FRotator rotationRate;

	// Scene component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	USceneComponent* sceneComponent;

	// Box collider.
	UPROPERTY(EditAnywhere)
	UBoxComponent* boxCollider;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
