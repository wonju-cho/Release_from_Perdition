// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ShadowRunnerCharacter.h"
#include "CheckPoint.generated.h"

UCLASS()
class SHADOWRUNNER_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	//	UStaticMeshComponent* pickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
		UStaticMeshComponent* topMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
		FRotator rotationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* boxCollider;

	// Arrow component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UArrowComponent* arrow;
	
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
};
