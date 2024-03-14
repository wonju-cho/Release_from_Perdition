// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultEnemy.h"

// Sets default values
ADefaultEnemy::ADefaultEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ratio
	EnemyHealth = 1.0f;

	SpawnPosition = GetActorLocation();
}

// Called when the game starts or when spawned
void ADefaultEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefaultEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADefaultEnemy::TakeDamage(float damage)
{
	EnemyHealth -= damage;
	if(EnemyHealth <= 0)
	{
		EnemyHealth = 0.f;
		Dead();
	}
}

void ADefaultEnemy::Dead()
{
	Respawn();
}

void ADefaultEnemy::Respawn()
{
	EnemyHealth = 1.0f;
	SetActorLocation(SpawnPosition);
}
