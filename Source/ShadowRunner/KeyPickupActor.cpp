// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyPickupActor.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowRunnerCharacter.h"
#include "FirstDoorActor.h"
#include "ShadowRunnerHUD.h"
#include "Containers/UnrealString.h"

extern float volumeControlEnemy;

// Sets default values
AKeyPickupActor::AKeyPickupActor()
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
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AKeyPickupActor::OnOverlapBegin);
	boxCollider->AttachTo(RootComponent); // Depricated, need to change it later.

	targetDoorId = 0; // 0 means not set.
}

// Called when the game starts or when spawned
void AKeyPickupActor::BeginPlay()
{
	Super::BeginPlay();
	particlePlay();

}

// Called every frame
void AKeyPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(rotationRate * DeltaTime);
}
void AKeyPickupActor::particlePlay()
{
	FVector location = GetActorLocation();

	if (Particle_ability)
	{
		particleComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle_ability, location);
	}
}
void AKeyPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AShadowRunnerCharacter* player = Cast<AShadowRunnerCharacter>(OtherActor);
		if (player)
		{
			// Get all door actors.
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFirstDoorActor::StaticClass(), FoundActors);

			if (targetDoorId)
			{
				for (int i = 0; i < FoundActors.Num(); ++i)
				{
					AFirstDoorActor* door = Cast<AFirstDoorActor>(FoundActors[i]);
					if (door)
					{
						if (door->keyId == targetDoorId)
						{
							door->UnlockDoor();

							if (doorUnlockedSound != nullptr)
							{
								UGameplayStatics::PlaySoundAtLocation(this, doorUnlockedSound, GetActorLocation(), GetActorRotation(), volumeControlEnemy);
							}

							// Display text on screen.
							AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
							if (shadowRunnerHUD)
							{
								shadowRunnerHUD->DisplayUnlocked();
							}
						}
					}
				}
			}

			Destroy();
			particleComp->DestroyInstance();
		}
	}
}