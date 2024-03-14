// Fill out your copyright notice in the Description page of Project Settings.


#include "AlexTestPickupActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAlexTestPickupActor::AAlexTestPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//RootComponent = sceneComponent;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	pickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pickupMesh"));
	pickupMesh->AttachTo(RootComponent);

	rotationRate = FRotator(0.0f, 180.0f, 0.0f);

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollider"));
	boxCollider->SetGenerateOverlapEvents(true);
	boxCollider->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AAlexTestPickupActor::OnOverlapBegin);
	//boxCollider->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AAlexTestPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Put it here to prevent warning and packaging error.
	//boxCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called every frame
void AAlexTestPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(rotationRate * DeltaTime);
}

void AAlexTestPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		TSubclassOf<UDamageType> damageType;
		UGameplayStatics::ApplyDamage(OtherActor, -25.0f, nullptr, this, damageType);
		Destroy();
	}
}