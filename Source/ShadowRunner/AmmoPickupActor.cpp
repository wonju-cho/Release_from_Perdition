// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickupActor.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowRunnerCharacter.h"
#include "ShadowRunnerHUD.h"

extern float volumeControlEnemy;

// Sets default values
AAmmoPickupActor::AAmmoPickupActor()
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
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickupActor::OnOverlapBegin);
	boxCollider->AttachTo(RootComponent); // Depricated, need to change it later.
}

// Called when the game starts or when spawned
void AAmmoPickupActor::BeginPlay()
{
	Super::BeginPlay();
	particlePlay();

	// Doesn't work now need to find solution later.
	// Put it here to prevent warning and packaging error.
	//boxCollider->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
}

// Called every frame
void AAmmoPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(rotationRate * DeltaTime);
}


void AAmmoPickupActor::particlePlay()
{
	FVector location = GetActorLocation();

	if (Particle_ability)
	{
		particleComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle_ability, location);
	}
}

void AAmmoPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Add ammo.
		AShadowRunnerCharacter* player = Cast<AShadowRunnerCharacter>(OtherActor);
		if (player)
		{
			//Increase the ammo of the first gun.
			player->weapons[0]->GetDefaultObject<ABaseWeapon>()->clipAmmo += player->weapon->GetDefaultObject<ABaseWeapon>()->addClipAmmo;

			// Update HUD.
			AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			
			//Update only for when it is the first gun.
			if (player->weaponIndex == 0)
			{
				shadowRunnerHUD->UpdateAmmo(player->weapons[player->weaponIndex]->GetDefaultObject<ABaseWeapon>()->clipAmmo, INFINITE, 10, player->weaponIndex); //Original Ammo
			}

			if (AmmoPickupSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, AmmoPickupSound, GetActorLocation(), volumeControlEnemy);
			}

			Destroy();
			particleComp->DestroyInstance();
		}
	}
}