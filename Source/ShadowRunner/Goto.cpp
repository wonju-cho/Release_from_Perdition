// Fill out your copyright notice in the Description page of Project Settings.


#include "Goto.h"

// Sets default values
AGoto::AGoto()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollider"));
	boxCollider->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	boxCollider->SetCollisionProfileName("Trigger");
	RootComponent = boxCollider;

	topMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("topMesh"));
	topMesh->AttachTo(RootComponent);

	//pickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pickupMesh"));
	//pickupMesh->AttachTo(RootComponent);

	rotationRate = FRotator(0.0f, 180.0f, 0.0f);

	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AGoto::OnOverlapBegin);
	//boxCollider->OnComponentEndOverlap.AddDynamic(this, &ACheckPoint::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AGoto::BeginPlay()
{
	Super::BeginPlay();
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AGoto::OnOverlapBegin);
}

// Called every frame
void AGoto::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	topMesh->AddLocalRotation(rotationRate * DeltaTime);
}

void AGoto::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AShadowRunnerCharacter* PlayerCharacter = Cast<AShadowRunnerCharacter>(OtherActor);

	if (OtherActor->ActorHasTag("ShadowRunnerCharacter"))
	{
		ChangeLevel();
	}
}

void AGoto::ChangeLevel()
{
	UWorld* TheWorld = GetWorld();

	FString CurrentLevel = TheWorld->GetMapName();

	if (CurrentLevel == "Prototype_Level")
	{
		UGameplayStatics::OpenLevel(GetWorld(), "GameEnding");
	}
	else if (CurrentLevel == "New_MainLevel") // Modified by alex.
	{
		UGameplayStatics::OpenLevel(GetWorld(), "GameEnding");
	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Prototype_Level");
	}
}