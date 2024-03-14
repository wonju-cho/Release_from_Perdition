// Fill out your copyright notice in the Description page of Project Settings.


#include "../AImagician/AIMagic.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "ShadowRunner/ShadowRunnerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShadowRunner/AImagician/AIMagicController.h"
#include "ShadowRunner/AImagician/AIMagic_blackBoard_Keys.h"
#include "BehaviorTree/BlackboardComponent.h"

extern float volumeControlEnemy;

FVector AAIMagic::getLocation()
{
	return SpawnLocation;
}
// Sets default values
AAIMagic::AAIMagic()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GunOffset = FVector(400.0f, 0.0f, 0.0f);
	health = 100;
	IsDead = false;
	SpawnLocation = FVector(0.f, 0.f, 0.f);

	prevHealth = health;

	// Timer variables.
	hitDisplayTime = 0.5f;
}

float AAIMagic::GetHealth()
{
	return health;
}

// Called when the game starts or when spawned
void AAIMagic::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAIMagic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	ACharacter* const player_ = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerLocations = player_->GetActorLocation();


	AAIMagicController* cont = Cast< AAIMagicController>(GetController());
	if (cont)
	{
		ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (player)
		{
			FVector player_location = player->GetActorLocation();
			cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::player_location, player_location);
		}
	}

	if (!checkCylinder)
	{
		checkCylinder = true;
		cont->get_blackBoard()->SetValueAsBool(magic_bb_keys::is_on_cylinder, OnCylinder);
	}

	// Check if the health changed.
	if (health != prevHealth)
	{
		if (!(cont->get_blackBoard()->GetValueAsBool(magic_bb_keys::can_see_player)))
		{
			if (cont)
			{
				cont->get_blackBoard()->SetValueAsBool(magic_bb_keys::got_attack_behind, true);
				//cont->get_blackBoard()->SetValueAsBool(magic_bb_keys::can_see_player, true);
			}
		}

		prevHealth = health;

		// Change enemy material for hit feedback.
		if (hitMaterial)
		{
			// Change the material to red for 0.1 second.
			if (!GetWorld()->GetTimerManager().IsTimerActive(hitTimerHandle))
			{
				this->GetMesh()->SetMaterial(0, hitMaterial);
				GetWorld()->GetTimerManager().SetTimer(hitTimerHandle, this, &AAIMagic::HitTimerCallback, hitDisplayTime, false);
			}
		}
	}

	if (health <= 0.0f)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);

		if (!IsDead)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DyingSound, GetActorLocation(), volumeControlEnemy*0.75f);
		}

		IsDead = true;

		if (IsDead && DyingSound != nullptr)
		{

			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetMesh()->SetSimulatePhysics(true);
			deathCount += DeltaTime;


			if (deathCount >= 1.5f)
			{
				deathCount = 0.f;
				int randomNum = FMath::RandRange(0, 100);

				FActorSpawnParameters ActorSpawnParams; //Set Spawn Collision Handling Override
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


				if (healthDropClass && ammoDropClass)
				{
					if (randomNum % 2 == 0 && randomNum != 0)
					{
						if (!Buff)
						{
							Buff = true;
							GetWorld()->SpawnActor<AHealthPickupActor>(healthDropClass, this->GetActorLocation(), this->GetActorRotation(), ActorSpawnParams);
						}
					}

					if (randomNum % 2 == 1 && randomNum != 0)
					{
						if (!Buff)
						{
							Buff = true;
							GetWorld()->SpawnActor<AAmmoPickupActor>(ammoDropClass, this->GetActorLocation(), this->GetActorRotation(), ActorSpawnParams);
						}
					}
				}

				cont->OnEnemyDeath();

				Destroy();
			}


		}



		//Destroy();
	}
}

void AAIMagic::MagicAttackPlay()
{
	if (montage)
	{
		if (HitSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(), volumeControlEnemy * 0.75f);
		}
		PlayAnimMontage(montage);
	}

	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(AttackHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(AttackHandle, this, &AAIMagic::Attack, 1.0f, false);
		}
	}

	//Attack();
}

void AAIMagic::Attack()
{
	AAIMagicController* cont = Cast< AAIMagicController>(GetController());

	bool shadow = cont->get_blackBoard()->GetValueAsBool(magic_bb_keys::can_see_shadow);//getValueAsVector(magic_bb_keys::player_location, player_location);

	if (shadow)
	{
		ShadowAttack();
	}
	else
	{
		particlePlay();

		ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		FRotator SpawnRotation = this->GetActorRotation();

		float x_ = this->GetActorLocation().X - playerLocations.X;
		float y_ = this->GetActorLocation().Y - playerLocations.Y;

		GunOffset.X = sqrt(x_ * x_ + y_ * y_);

		SpawnLocation = this->GetActorLocation() + SpawnRotation.RotateVector(GunOffset);
		SpawnLocation.Z = playerLocations.Z - 98.f;
		//FVector rotation = SpawnRotation.RotateVector(GunOffset);
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FTransform(SpawnRotation, SpawnLocation));
			//DrawDebugLine(GetWorld(), this->GetActorLocation(), SpawnLocation, FColor::Red, false, 0.5, 0, 5);
		}

		float x = SpawnLocation.X - playerLocations.X;
		float y = SpawnLocation.Y - playerLocations.Y;

		if (sqrt(x * x + y * y) <= 500.f)
		{
			ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			TSubclassOf<UDamageType> damageType;
			UGameplayStatics::ApplyDamage(PlayerCharacter, 5.0f, nullptr, this, damageType);
		}

		if (cont)
		{
			cont->get_blackBoard()->SetValueAsBool(magic_bb_keys::got_attack_behind, false);

		}

	}

}

void AAIMagic::ShadowAttack()
{
	particlePlay();

	AAIMagicController* cont = Cast< AAIMagicController>(GetController());
	if (cont)
	{
		cont->get_blackBoard()->SetValueAsBool(magic_bb_keys::got_attack_behind, false);
	}

	FVector shadowLocation = cont->get_blackBoard()->GetValueAsVector(magic_bb_keys::shadow_location);

	//ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FRotator SpawnRotation = this->GetActorRotation();

	float x_ = this->GetActorLocation().X - shadowLocation.X;
	float y_ = this->GetActorLocation().Y - shadowLocation.Y;

	GunOffset.X = sqrt(x_ * x_ + y_ * y_);

	SpawnLocation = this->GetActorLocation() + SpawnRotation.RotateVector(GunOffset);
	SpawnLocation.Z = shadowLocation.Z - 98.f;//shadowLocation.Z - 98.f;
	//{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FTransform(SpawnLocation));
	//}

	//float x = SpawnLocation.X - playerLocations.X;
	//float y = SpawnLocation.Y - playerLocations.Y;

	//if (sqrt(x * x + y * y) <= 250.f)
	//{
	//	ACharacter* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//	TSubclassOf<UDamageType> damageType;
	//	UGameplayStatics::ApplyDamage(PlayerCharacter, 5.0f, nullptr, this, damageType);
	//}

}




void AAIMagic::particlePlay()
{
	FVector location = GetActorLocation();

	if (Particle_ability)
	{
		particleComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle_ability, location);
	}
}

UAnimMontage* AAIMagic::GetMontage() const
{
	return montage;
}

// Called to bind functionality to input
void AAIMagic::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAIMagic::HitTimerCallback()
{
	this->GetMesh()->SetMaterial(0, baseMaterial);
}
