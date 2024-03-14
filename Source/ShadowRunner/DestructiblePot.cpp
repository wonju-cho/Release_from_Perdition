// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructiblePot.h"
#include "ShadowRunnerCharacter.h"
#include "Engine.h"

extern float volumeControlEnemy;

// Sets default values
ADestructiblePot::ADestructiblePot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	
	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Component"));
	DestructibleComponent->SetupAttachment(RootComponent);
	DestructibleComponent->SetNotifyRigidBodyCollision(true);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Component"));
	TriggerComponent->SetupAttachment(RootComponent);

	IsDestroied = false;
	IsTriggerEnabled = false;

	MaxHealth = 10.f;
	DefaultDamage = 1.f;
	DefaultImpulse = 1.f;

}

// Called when the game starts or when spawned
void ADestructiblePot::BeginPlay()
{
	Super::BeginPlay();

	DestructibleComponent->OnComponentHit.AddDynamic(this, &ADestructiblePot::Damage);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestructiblePot::Trigger);
	CurrentHealth = MaxHealth;
	
}

// Called every frame
void ADestructiblePot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADestructiblePot::Damage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, OtherComp->GetName());
	
	//AShadowRunnerProjectile* bullet = Cast<AShadowRunnerProjectile>(Hit.GetActor());

	if (!IsDestroied && OtherActor->ActorHasTag("Bullet"))
	{
		UE_LOG(LogTemp, Warning, TEXT("damage with first bullet"));
		//Destroy(DefaultDamage, Hit.Location, NormalImpulse, DefaultImpulse);
	}

}

void ADestructiblePot::Trigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, __FUNCTION__);
	if (!IsDestroied && IsTriggerEnabled && OtherActor->ActorHasTag("Bullet"))
	{
		UE_LOG(LogTemp, Warning, TEXT("trigger_bullet"));
		//Destroy();
	}
	
	if (!IsDestroied && IsTriggerEnabled && OtherActor->ActorHasTag("ShadowRunnerCharacter"))
	{
		UE_LOG(LogTemp, Warning, TEXT("trigger_character"));
		//Destroy();
	}
}

void ADestructiblePot::Destroy(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse)
{
	if (!IsDestroied)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pot is destroied"));

		IsDestroied = true;
		DestructibleComponent->ApplyDamage(Damage, HitLocation, ImpulseDir, Impulse);

		GetWorldTimerManager().SetTimer(TimerForDisappear, this, &ADestructiblePot::DisappearPot, 0.1f, false);
		
		UGameplayStatics::PlaySoundAtLocation(this, breaking_pot_sound, GetActorLocation(), volumeControlEnemy);
	}
}

void ADestructiblePot::DisappearPot()
{
	randomNum = FMath::RandRange(0, 100);

	if (this != nullptr)
	{
		FVector SpawnLocation(this->GetActorLocation() + FVector(0.f, 0.f, 100.f));

		FActorSpawnParameters spawnParams;

		if (randomNum <= 30) // 30%
		{
			UE_LOG(LogTemp, Warning, TEXT("Bullet spawner"));
			item_1_Actor_Ref = GetWorld()->SpawnActor<AActor>(item_1, SpawnLocation, this->GetActorRotation(), spawnParams);
		}
		else if (randomNum <= 80) //50%
		{
			UE_LOG(LogTemp, Warning, TEXT("HealPack spawner"));
			item_2_Actor_Ref = GetWorld()->SpawnActor<AActor>(item_2, SpawnLocation, this->GetActorRotation(), spawnParams);
		}

		GetWorldTimerManager().SetTimer(TimerForDisappear, this, &ADestructiblePot::DestroyingMesh, 1.f, false);
	}
}

void ADestructiblePot::DestroyingMesh()
{
	DestructibleComponent->DestroyComponent();
}
