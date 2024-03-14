// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShadowRunnerProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FlyingEnemyCharacter.h"
#include "ShadowRunnerCharacter.h"
#include "ShadowRunner/AImagician/AIMagic.h"
#include "DestructiblePot.h"

extern float volumeControlEnemy;

AShadowRunnerProjectile::AShadowRunnerProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AShadowRunnerProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 6000.f;
	ProjectileMovement->MaxSpeed = 6000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	// AOE radius.
	AOERadius = 300.0f;

	// Damage.
	damage = 25.0f;
}

void AShadowRunnerProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	//if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Projectile hit")));

#if 0
		// When hitting flying enemy.
		AFlyingEnemyCharacter* flyingEnemy = Cast<AFlyingEnemyCharacter>(OtherActor);
		if (flyingEnemy)
		{
			flyingEnemy->health -= 25;
		}

		// When hitting the player.
		AShadowRunnerCharacter* PlayerCharacter = Cast<AShadowRunnerCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			TSubclassOf<UDamageType> damageType;
			UGameplayStatics::ApplyDamage(PlayerCharacter, 5.0f, nullptr, this, damageType);
		}

		AAIMagic* magicianEnemy = Cast<AAIMagic>(OtherActor);
		if (magicianEnemy)
		{
			UGameplayStatics::PlaySoundAtLocation(this, GethurtSound, GetActorLocation(), volumeControlEnemy);

			magicianEnemy->health -= 25.f;
		}
#endif

		// Apply damage to melee enemies nearby.
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundActors);
		for (int i = 0; i < FoundActors.Num(); ++i)
		{
			float distance = FVector::Distance(this->GetActorLocation(), FoundActors[i]->GetActorLocation());

			if (distance <= AOERadius)
			{
				//TSubclassOf<UDamageType> damageType;
				//UGameplayStatics::ApplyDamage(FoundActors[i], 100, nullptr, this, damageType);

				AEnemyCharacter* melee = Cast<AEnemyCharacter>(FoundActors[i]);
				if (melee)
				{
					//if (melee != OtherActor)
					//{
						melee->TakeDamage(damage);
					//}
				}
			}
		}

		// Apply damage to flying enemies nearby.
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFlyingEnemyCharacter::StaticClass(), FoundActors);
		for (int i = 0; i < FoundActors.Num(); ++i)
		{
			float distance = FVector::Distance(this->GetActorLocation(), FoundActors[i]->GetActorLocation());

			if (distance <= AOERadius)
			{
				//TSubclassOf<UDamageType> damageType;
				//UGameplayStatics::ApplyDamage(FoundActors[i], 100, nullptr, this, damageType);

				AFlyingEnemyCharacter* flyingEnemy = Cast<AFlyingEnemyCharacter>(FoundActors[i]);
				if (flyingEnemy)
				{
					flyingEnemy->health -= damage;
				}
			}
		}

		// Apply damage to magician enemies nearby.
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIMagic::StaticClass(), FoundActors);
		for (int i = 0; i < FoundActors.Num(); ++i)
		{
			float distance = FVector::Distance(this->GetActorLocation(), FoundActors[i]->GetActorLocation());

			if (distance <= AOERadius)
			{
				//TSubclassOf<UDamageType> damageType;
				//UGameplayStatics::ApplyDamage(FoundActors[i], 100, nullptr, this, damageType);

				AAIMagic* magicianEnemy = Cast<AAIMagic>(FoundActors[i]);
				if (magicianEnemy)
				{
					UGameplayStatics::PlaySoundAtLocation(this, GethurtSound, GetActorLocation(), volumeControlEnemy);
					magicianEnemy->health -= damage;
				}
			}
		}

		// Emit particle.
		if (ImpactParticles)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in particle")));

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, HitComp->GetComponentLocation());
		}

		ADestructiblePot* pot = Cast<ADestructiblePot>(OtherActor);
		if (pot)
		{
		    UE_LOG(LogTemp, Warning, TEXT("damage with first bullet"));
		    pot->Destroy(pot->DefaultDamage, Hit.Location, this->GetActorForwardVector(), pot->DefaultImpulse);
		}

		Destroy();
	}
}