// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));

	RootComponent = platform;

	delta = 0.f;
	velocity = 8.f;
	range = 1.f;
	currPos = FVector(0.f);

	platform->SetGenerateOverlapEvents(true);
	platform->SetCollisionObjectType(ECC_GameTraceChannel1);
	platform->SetCollisionResponseToAllChannels(ECR_Block);

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	currPos = platform->GetComponentLocation();
	platform->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	delta += DeltaTime;
	
	currPos.Z += range * FMath::Sin(delta * velocity);

	platform->SetWorldLocation(currPos);

}

