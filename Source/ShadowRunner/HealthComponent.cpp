// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "ShadowRunnerCharacter.h"
#include "ShadowRunnerSlateHUD.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Set default health.
	defaultHealth = 150;
	health = defaultHealth;
	
	UE_LOG(LogTemp, Warning, TEXT("Health Component Initialized: DefaultHealth: %f, Health: %f"), defaultHealth, health);

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
	// UGameplayStatics::ApplyDamage(PlayerCharacter, 5.0f, nullptr, this, damageType);
	// Setup i-frame.
	AShadowRunnerCharacter* player = Cast<AShadowRunnerCharacter>(DamagedActor);
	if (!GetWorld()->GetTimerManager().IsTimerActive(iFrameTimerHandle) || player)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("IN DAMEGE: health is %f"), health));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("ffffffffffffff")));
		
		// Update HUD.
		if(player)
		{
			//UE_LOG(LogTemp, Warning, TEXT("defaultHealth: %d"), defaultHealth);
			const float remainHP = FMath::Clamp(health - Damage, 0.0f, defaultHealth);
			if(DamageCauser->Tags.Contains(TEXT("Melee")) || DamageCauser->Tags.Contains(TEXT("Maigician"))){
			}
				// UE_LOG(LogTemp, Warning, TEXT("health : %d \t health - Damage: %d \t defaultHealth: %d"), health, remainHP, defaultHealth);
				// UE_LOG(LogTemp, Warning, TEXT("health: %d"), health);
				health = remainHP;
				player->GetHUD()->UpdateHealth(health, health);
		}
		
		GetWorld()->GetTimerManager().SetTimer(iFrameTimerHandle, this, &UHealthComponent::IFrameCallback, iFrameTime, false);
	}
}

void UHealthComponent::UpdateHUD()
{
	AShadowRunnerSlateHUD* shadowRunnerHUD = Cast<AShadowRunnerSlateHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	shadowRunnerHUD->UpdateHealth(health, health);
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