// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingEnemyCharacter.h"
#include "ShadowFlyEnemySpawn.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "FlyingEnemyAIController.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Misc/OutputDeviceDebug.h"
#include "ShadowRunnerCharacter.h"
#include "GameFramework/GameStateBase.h"

extern float volumeControlEnemy;

// Sets default values
AFlyingEnemyCharacter::AFlyingEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("pawnSensingComponent"));
	pawnSensingComponent->SetPeripheralVisionAngle(180.0f);
	currentTarget = nullptr;

	// Setup AI controller.
	AIControllerClass = AFlyingEnemyAIController::StaticClass();

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 50.0f);

	// Initial health.
	health = 10;
	flySpeed = 600.0f;
	waveTurnRate = 3.0f;
	chargeSpeed = 3000.0f;
	prevHealth = health;

	// Damage dealt.
	damage = 50.0f;

	// Timer variables.
	hitDisplayTime = 0.5f;
	deathAnimTime = 1.5f;

	// Setup box collider.
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollider"));
	boxCollider->SetupAttachment(this->GetCapsuleComponent());
	boxCollider->SetGenerateOverlapEvents(true);
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFlyingEnemyCharacter::OnOverlapBegin);

	// Flags.
	bScreamAnimPlaying = false;
	bChargeAnimPlaying = false;
	bMoveAnimiPlaying = false;
	bDeathAnimPlaying = false;
	bDead = false;

	// Movement component.
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	GetCharacterMovement()->GroundFriction = 0.0f;
}

// Called when the game starts or when spawned
void AFlyingEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (pawnSensingComponent)
	{
		pawnSensingComponent->OnSeePawn.AddDynamic(this, &AFlyingEnemyCharacter::OnPlayerDetected);
	}
}

// Called every frame
void AFlyingEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (currentTarget)
	{
		// Check if the target is still in the sensing range.
		float distance = FVector::Dist(this->GetActorLocation(), currentTarget->GetActorLocation());
		if (distance > pawnSensingComponent->SightRadius)
		{
			AFlyingEnemyAIController* AIController = Cast<AFlyingEnemyAIController>(GetController());

			if (AIController)
			{
				AIController->SetPlayerDetected(nullptr);
				currentTarget = nullptr;
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("player null")));
			}
		}
	}

	// Check if the health changed.
	if (health != prevHealth)
	{
		prevHealth = health;

		// Change enemy material for hit feedback.
		if (hitMaterial)
		{
			// Change the material to red for 0.1 second.
			if (!GetWorld()->GetTimerManager().IsTimerActive(hitTimerHandle))
			{
				this->GetMesh()->SetMaterial(0, hitMaterial);
				GetWorld()->GetTimerManager().SetTimer(hitTimerHandle, this, &AFlyingEnemyCharacter::HitTimerCallback, hitDisplayTime, false);
			}
		}
	}

	// Check health.
	if (health <= 0.0f)
	{
    //if(DieSound != nullptr)
		//{
		//	UGameplayStatics::PlaySoundAtLocation(this, DieSound, GetActorLocation());
		//}
    
#if 0 // The pickup drop now will float in the mid-air, need to fix the pickup physics first.
		// drop pickups.
		FRotator SpawnRotation = this->GetActorRotation();
		FVector SpawnLocation = this->GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

		FActorSpawnParameters ActorSpawnParams; //Set Spawn Collision Handling Override
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (healthDropClass && ammoDropClass)
		{
			GetWorld()->SpawnActor<AHealthPickupActor>(healthDropClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			GetWorld()->SpawnActor<AAmmoPickupActor>(ammoDropClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
#endif
		//Destroy();
		DeathSequence();
		// For audio system.
		// AGameStateBase* gameState = GetWorld()->GetGameState();
		// const FString command = FString::Printf(TEXT("Update_Enemy_Count %d"), -1);
		// FOutputDeviceDebug debug;
		// gameState->CallFunctionByNameWithArguments(*command, debug, this, true);
		// -------------------------------------------
		GetMesh()->SetCollisionProfileName("Ragdoll");
		GetMesh()->SetSimulatePhysics(true);
	}
}

// Called to bind functionality to input
void AFlyingEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFlyingEnemyCharacter::SpawnProjectile()
{
	FRotator SpawnRotation = this->GetActorRotation();
	FVector SpawnLocation = this->GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn projectile.
	GetWorld()->SpawnActor<AShadowRunnerProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
  
  if(AttackSound != nullptr)
  {
    UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation(), volumeControlEnemy * 0.65f);
	}
}

void AFlyingEnemyCharacter::OnPlayerDetected(APawn* pawn)
{
	AFlyingEnemyAIController* AIController = Cast<AFlyingEnemyAIController>(GetController());
	
	if (AIController)
	{
		// Check if target point is blocked.
		FVector start = this->GetActorLocation();
		FVector end = pawn->GetActorLocation();
		FHitResult hitResult;
		FCollisionQueryParams collisionParams;

		bool IsBlocked = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, collisionParams);
		if (!IsBlocked)
		{
			AIController->SetPlayerDetected(pawn);
			currentTarget = pawn;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("player detected")));
	}
}

void AFlyingEnemyCharacter::DeathSequence()
{
	if (!bDead)
	{
		AFlyingEnemyAIController* AIController = Cast<AFlyingEnemyAIController>(GetController());
		if (AIController)
		{
			AIController->BrainComponent->StopLogic(FString::Printf(TEXT("Flying Enemy Dead")));
		}

		UMovementComponent* MovementComponent = Cast<UMovementComponent>(this->GetComponentByClass(UMovementComponent::StaticClass()));
		if (MovementComponent)
		{
			MovementComponent->Velocity = FVector(0.0f);
		}

		bDead = true;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->GetName());
	PlayDeathAnimation();

	// Set timer for death animation.
	if (!GetWorld()->GetTimerManager().IsTimerActive(deathTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &AFlyingEnemyCharacter::DeathTimerCallback, deathAnimTime, false);
	}
}

float AFlyingEnemyCharacter::GetEnemyHealth()
{
	float healthCheck = 0;
	if (!bDeathAnimPlaying)
	{
		healthCheck++;
	}
	else
	{
		healthCheck = 0;
	}
	return healthCheck;
}

void AFlyingEnemyCharacter::HitTimerCallback()
{
	this->GetMesh()->SetMaterial(0, baseMaterial);
}

void AFlyingEnemyCharacter::DeathTimerCallback()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->GetName());

	// For audio system.
	//AGameStateBase* gameState = GetWorld()->GetGameState();
	//const FString command = FString::Printf(TEXT("Update_Enemy_Count %d"), -1);
	//FOutputDeviceDebug debug;
	//gameState->CallFunctionByNameWithArguments(*command, debug, this, true);
	// -------------------------------------------

	AFlyingEnemyAIController* AIController = Cast<AFlyingEnemyAIController>(GetController());
	if (AIController)
	{
		AIController->OnEnemyDeath();
	}
	
	this->Destroy();
}

void AFlyingEnemyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("HIT")));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->GetName());
	AShadowRunnerCharacter* player = Cast<AShadowRunnerCharacter>(OtherActor);
	if (player && !bDead)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("BOOM")));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->GetName());

		// Apply damage to player.
		TSubclassOf<UDamageType> damageType;
		UGameplayStatics::ApplyDamage(player, damage, nullptr, this, damageType);

		//health = 0.0f;

		//Destroy();
		DeathSequence();
	}
}

void AFlyingEnemyCharacter::PlayScreamAnimation()
{
	if (screamAnimation && !bScreamAnimPlaying)
	{
		this->GetMesh()->PlayAnimation(screamAnimation, false);
		bScreamAnimPlaying = true;
	}
}

void AFlyingEnemyCharacter::PlayChargeAnimation()
{
	if (chargeAnimation && !bChargeAnimPlaying)
	{
		this->GetMesh()->PlayAnimation(chargeAnimation, false);
		bChargeAnimPlaying = true;
	}
}

void AFlyingEnemyCharacter::PlayMoveAnimation()
{
	if (moveAnimation && !bMoveAnimiPlaying)
	{
		this->GetMesh()->PlayAnimation(moveAnimation, true);
		bMoveAnimiPlaying = true;
	}
}

void AFlyingEnemyCharacter::PlayDeathAnimation()
{
	if (deathAnimation && !bDeathAnimPlaying)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->GetName());
		this->GetMesh()->PlayAnimation(deathAnimation, false);
		bDeathAnimPlaying = true;

		if (DieSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DieSound, GetActorLocation(), volumeControlEnemy * 0.65f);
		}

		// Emit particles.
		if (explodeParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explodeParticles, FTransform(this->GetActorRotation(), this->GetActorLocation()));
		}

		// Play sound effect.
		if (ExplodeSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, this->ExplodeSound, this->GetActorLocation(), volumeControlEnemy * 0.65f);
		}
	}
}

void AFlyingEnemyCharacter::ResetAnimFlags()
{
	bScreamAnimPlaying = false;
	bChargeAnimPlaying = false;
	bMoveAnimiPlaying = false;
}

float AFlyingEnemyCharacter::GetFlySpeed()
{
	return flySpeed;
}

float AFlyingEnemyCharacter::GetWaveTurnRate()
{
	return waveTurnRate;
}

float AFlyingEnemyCharacter::GetChargeSpeed()
{
	return chargeSpeed;
}