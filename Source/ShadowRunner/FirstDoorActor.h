// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirstDoorActor.generated.h"

UCLASS()
class SHADOWRUNNER_API AFirstDoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirstDoorActor();

	enum class DoorState
	{
		OPENING,
		CLOSING,
		OPENED,
		CLOSED
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ToggleDoor(FVector characterFacing);

	UFUNCTION()
	void UnlockDoor();

	// Meshes.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorMesh")
	class UStaticMeshComponent* leftDoorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorMesh")
	class UStaticMeshComponent* rightDoorMesh;

	// Collider.
	//UPROPERTY(EditAnywhere, Category = "Collider")
	//class UBoxComponent* boxCollider;

	// Arrow component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UArrowComponent* arrow;

	// ID.
	UPROPERTY(EditAnywhere, Category = "Identifiers")
	int keyId;

	UPROPERTY(EditAnywhere, Category = "Identifiers")
	int doorId;
	
	// Door state.
	DoorState doorState;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* DoorOpenSound;
    
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* DoorLockedSound;
  
	// Math.
	float dotProduct;
	float maxDegree;
	float minDegree;
	float leftAddYaw;
	float rightAddYaw;
	float leftSign;
	float rightSign;
	float directionSign;
	float leftDoorCurrentRotation;
	float rightDoorCurrentRotation;

private:
	void CloseDoor(float DeltaTime);

	void OpenDoor(float DeltaTime);
};
