// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "ShadowRunnerHUD.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Set default health.
	defaultHealth = 150;
	health = defaultHealth;

	iFrameTime = 1.0f;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get the actor who owns this health bar component.
	AActor* owner = GetOwner();
	if (owner)
	{
		owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// Setup i-frame.
	AShadowRunnerCharacter* player = Cast<AShadowRunnerCharacter>(DamageCauser);
	if (!GetWorld()->GetTimerManager().IsTimerActive(iFrameTimerHandle) || player)
	{
		health = FMath::Clamp(health - Damage, 0.0f, defaultHealth);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("IN DAMEGE: health is %f"), health));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("ffffffffffffff")));

		// Update HUD.
		UpdateHUD();

		GetWorld()->GetTimerManager().SetTimer(iFrameTimerHandle, this, &UHealthComponent::IFrameCallback, iFrameTime, false);
	}
}

void UHealthComponent::UpdateHUD()
{
	AShadowRunnerHUD* shadowRunnerHUD = Cast<AShadowRunnerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	shadowRunnerHUD->UpdateHealth(health, defaultHealth);
}

float UHealthComponent::GetHealth()
{
	return health;
}

void UHealthComponent::SetHealth(float newHealth)
{
	health = newHealth;
}

void UHealthComponent::SetIsHealthCheat(bool newIsHealthCheat)
{
	isHealthCheat = newIsHealthCheat;
}

void UHealthComponent::ResetHealth()
{
	defaultHealth = 150;
	health = defaultHealth;
}

float UHealthComponent::GetDefaultHealth()
{
	return defaultHealth;
}

void UHealthComponent::IFrameCallback()
{

}