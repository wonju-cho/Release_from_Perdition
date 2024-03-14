// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleJumpPickupActor.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowRunnerCharacter.h"
#include "ShadowRunnerHUD.h"


extern float volumeControlEnemy;
// Sets default values
ADoubleJumpPickupActor::ADoubleJumpPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//RootComponent = sceneComponent;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	pickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pickupMesh"));
	pickupMesh->AttachTo(RootComponent); // Depricated, need to change it later.

	rotationRate = FRotator(0.0f, 180.0f, 0.0f);

	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollider"));
	boxCollider->SetGenerateOverlapEvents(true);
	boxCollider->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &ADoubleJumpPickupActor::OnOverlapBegin);
	boxCollider->AttachTo(RootComponent); // Depricated, need to change it later.
}

// Called when the game starts or when spawned
void ADoubleJumpPickupActor::BeginPlay()
{
	Super::BeginPlay();
	particlePlay();
}

// Called every frame
void ADoubleJumpPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(rotationRate * DeltaTime);

}

void ADoubleJumpPickupActor::particlePlay()
{
	FVector location = GetActorLocation();

	if (Particle_ability)
	{
		particleComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle_ability, location);
	}
}

void ADoubleJumpPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Add ammo.
		AShadowRunnerCharacter* player = Cast<AShadowRunnerCharacter>(OtherActor);
		if (player)
		{
			// Set jump number to 2.
			player->jumpNumber = 2;

			// Update HUD.
			AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			shadowRunnerHUD->UpdateAbilities(player);
			if (AbilityPickupSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, AbilityPickupSound, GetActorLocation(), volumeControlEnemy * 0.5f);
			}
			Destroy();
			particleComp->DestroyInstance();
		}
	}
}