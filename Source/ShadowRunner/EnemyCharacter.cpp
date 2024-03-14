 // Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"

#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowRunnerCharacter.h"
#include "EnemyAIController.h"
#include "ShadowRunnerHUD.h"
#include "ShadowCloneCharacter.h"
//#include "AITags.h"
#include "BlackBoardKeys.h"
#include "BehaviorTree/BlackBoardComponent.h"

extern float volumeControlEnemy;

 // Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	AttackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Hit Box"));

	AttackHitBox->SetupAttachment(GetMesh(), FName("RightHandSocket"));

	GetCharacterMovement()->MaxWalkSpeed = 375.f;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	EnemyMaxHealth = 125;
	
	EnemyHealth = EnemyMaxHealth;

	IsDead = false;

	HasTakenDamage = false;

	buffActorRef = nullptr;

	damageTimerFlag = false;

	hitDisplayTime = 0.5f;

	deathCount = 0;

	buffSpawned = false;
}

float AEnemyCharacter::GetEnemyHealth()
{
	return EnemyHealth;
}

void AEnemyCharacter::SetEnemyHealth(float newHeatlh)
{
	EnemyHealth = newHeatlh;
}

float AEnemyCharacter::GetEnemyMaxHealth()
{
	return EnemyMaxHealth;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	MontageLength = 3.f;

	AttackHitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnDealDamageToPlayerBeginOverlap);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Enemy health %f"), EnemyMaxHealth));
	if (IsDead)
	{
		GetMesh()->SetCollisionProfileName("Ragdoll");
		GetMesh()->SetSimulatePhysics(true);
		deathCount += DeltaTime;

		if (deathCount >= 1.5f)
		{
			// Adam's modifications
			Cast<AEnemyAIController>(GetController())->OnEnemyDeath();
			//------------------------
			Destroy();
		}
	}

	if(!damageTimerFlag)
	{
		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, [&]()
		{
			damageTimerFlag = false;
		}, 1.1f, false);
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

APatrolPath* AEnemyCharacter::GetPatrolPath()
{
	return PatrolPath;
}

 void AEnemyCharacter::OnDealDamageToPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	 AShadowRunnerCharacter* PlayerCharacter = Cast<AShadowRunnerCharacter>(OtherActor);
	 AShadowCloneCharacter* shadow = Cast<AShadowCloneCharacter>(OtherActor);

	if (PlayerCharacter && damageTimerFlag == false) {
		//deal damage to the player
		TSubclassOf<UDamageType> damageType;
		UGameplayStatics::ApplyDamage(PlayerCharacter, 15.0f, nullptr, this, damageType);
		damageTimerFlag = true;
	}

	if(shadow)
	{
		shadow->health -= 25.0f;
	}
}


 void AEnemyCharacter::MeleeAttack()
 {
	if (Montage)
	{
		if(HitSound != nullptr && !IsDead)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(), volumeControlEnemy*0.75);
		}
		PlayAnimMontage(Montage);
	}
 }

UAnimMontage* AEnemyCharacter::GetMontage() const
{
	return Montage;
}

void AEnemyCharacter::TakeDamage(float damage)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("------------------------"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Prev HP: %f"), GetEnemyHealth()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Damage: %f"), damage));

	EnemyHealth -= damage;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("After HP: %f"), GetEnemyHealth()));

	// Change enemy material for hit feedback.
	if (hitMaterial)
	{
		// Change the material to red for 0.1 second.
		if (!GetWorld()->GetTimerManager().IsTimerActive(hitTimerHandle))
		{
			this->GetMesh()->SetMaterial(0, hitMaterial);
			GetWorld()->GetTimerManager().SetTimer(hitTimerHandle, this, &AEnemyCharacter::HitTimerCallback, hitDisplayTime, false);
		}
	}

	if(OnHitSound != nullptr)
	{
  		UGameplayStatics::PlaySoundAtLocation(this, OnHitSound, GetActorLocation(), volumeControlEnemy* 0.75);
	}
  
	if (EnemyHealth <= 0)
	{
		IsDead = true;

		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);

		AttackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		

		randomNum = FMath::RandRange(0, 100);

		if (IsDead && DyingSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DyingSound, GetActorLocation(), volumeControlEnemy* 0.75);
		}
		
		if (this != nullptr)
		{
			SpawnLocation = this->GetActorLocation();

#if 0
			if (randomNum % 3 == 0 && randomNum != 0 && SpawnCount == 0)
			{
				FActorSpawnParameters spawnParams;

				SpawnCount++;

				buffActorRef = GetWorld()->SpawnActor<AActor>(buffClone, SpawnLocation, this->GetActorRotation(), spawnParams);
			}

			if (randomNum % 3 == 0 && randomNum != 0 && SpawnCount == 0)
			{
				SpawnCount++;

				FActorSpawnParameters spawnParams2;

				buffActorRef2 = GetWorld()->SpawnActor<AActor>(buffClone2, SpawnLocation, this->GetActorRotation(), spawnParams2);
			}
#endif
			// Alex modified.
			FActorSpawnParameters spawnParams;
			if (!buffSpawned)
			{
				if (randomNum <= 33)
				{
					buffActorRef = GetWorld()->SpawnActor<AActor>(buffClone, SpawnLocation, this->GetActorRotation(), spawnParams);
					buffSpawned = true;
				}
				else if (randomNum <= 66)
				{
					buffActorRef2 = GetWorld()->SpawnActor<AActor>(buffClone2, SpawnLocation, this->GetActorRotation(), spawnParams);
					buffSpawned = true;
				}
			}
		}

	  }
	  else {
		  HasTakenDamage = true;
	  }
}

void AEnemyCharacter::HitTimerCallback()
{
	this->GetMesh()->SetMaterial(0, baseMaterial);
}


float AEnemyCharacter::GetVolumes()
{
	return soundVolume;
}

void AEnemyCharacter::SetVolumes(float s)
{
	soundVolume = s;
}
