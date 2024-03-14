// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"
#include "Engine/Engine.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollider"));
	boxCollider->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	boxCollider->SetCollisionProfileName("Trigger");
	RootComponent = boxCollider;
	
	topMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("topMesh"));
	topMesh->AttachTo(RootComponent);

	// Arrow component.
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrow->SetupAttachment(RootComponent);

	//pickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pickupMesh"));
	//pickupMesh->AttachTo(RootComponent);

	rotationRate = FRotator(0.0f, 180.0f, 0.0f);

	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlapBegin);
	//boxCollider->OnComponentEndOverlap.AddDynamic(this, &ACheckPoint::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();


	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlapBegin);
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//AddActorLocalRotation(rotationRate * DeltaTime);
	topMesh->AddLocalRotation(rotationRate * DeltaTime);
}

void ACheckPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	

	AShadowRunnerCharacter* PlayerCharacter = Cast<AShadowRunnerCharacter>(OtherActor);
	
	if (OtherActor->ActorHasTag("ShadowRunnerCharacter"))
	{
		PlayerCharacter->originMesh = GetActorLocation();

		// Set respawn facing.
		PlayerCharacter->respawnFacing = arrow->GetForwardVector().Rotation();
	}

}
