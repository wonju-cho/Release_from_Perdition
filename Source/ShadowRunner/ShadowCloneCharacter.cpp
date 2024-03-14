// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowCloneCharacter.h"
#include "AIShadowCloneController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "ShadowRunnerProjectile.h"
#include "ShadowRunnerCharacter.h"

// Sets default values
AShadowCloneCharacter::AShadowCloneCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("pawnSensingComponent"));
	pawnSensingComponent->SetPeripheralVisionAngle(180.0f);
	currentTarget = nullptr;

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	health = 200.0f;
}

// Called when the game starts or when spawned
void AShadowCloneCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (pawnSensingComponent)
	{
		pawnSensingComponent->OnSeePawn.AddDynamic(this, &AShadowCloneCharacter::OnEnemyDetected);
	}

}

// Called every frame
void AShadowCloneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if 0
	if (currentTarget)
	{
		// Check if the target is still in the sensing range.
		float distance = FVector::Dist(this->GetActorLocation(), currentTarget->GetActorLocation());
		if (distance > pawnSensingComponent->SightRadius)
		{
			AAIShadowCloneController* AIController = Cast<AAIShadowCloneController>(GetController());

			if (AIController)
			{
				AIController->SetEnemyDetected(nullptr);
				currentTarget = nullptr;
			}
		}
	}
#endif
}

// Called to bind functionality to input
void AShadowCloneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AShadowCloneCharacter::SpawnProjectile()
{
	FRotator SpawnRotation = this->GetActorRotation();
	FVector SpawnLocation = this->GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn projectile.
	GetWorld()->SpawnActor<AShadowRunnerProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
}

void AShadowCloneCharacter::OnEnemyDetected(APawn* pawn)
{
#if 0 // Disable attacking enemy for now.
	AAIShadowCloneController* AIController = Cast<AAIShadowCloneController>(GetController());

	if (AIController)
	{
		if (pawn)
		{
			AShadowRunnerCharacter* player = Cast<AShadowRunnerCharacter>(pawn);
			if (!player) // Make it so that the shadow won't attack the player.
			{
				AIController->SetEnemyDetected(pawn);
				currentTarget = pawn;
			}
		}
	}
#endif
}