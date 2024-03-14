// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHADOWRUNNER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float defaultHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float health;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:
	// Timer handle for i frame.
	struct FTimerHandle iFrameTimerHandle;

	float iFrameTime;

	float GetHealth();

	void ResetHealth();

	float GetDefaultHealth();

	void UpdateHUD();

	void IFrameCallback();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float newHealth);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetIsHealthCheat(bool newIsHealthCheat);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool isHealthCheat = false;
};
