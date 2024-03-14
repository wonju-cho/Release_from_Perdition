// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DestructibleComponent.h"
#include "ShadowRunnerProjectile.h"
#include "Sound/SoundBase.h"

#include "DestructiblePot.generated.h"

UCLASS()
class SHADOWRUNNER_API ADestructiblePot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructiblePot();

private:
	int randomNum;
	AActor* item_1_Actor_Ref;
	AActor* item_2_Actor_Ref;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> item_1;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> item_2;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DestructiblePotSound)
		USoundBase* breaking_pot_sound;

	UFUNCTION()
	void Damage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void Trigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void Destroy(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse);

	UFUNCTION()
	void DisappearPot();

	UFUNCTION()
	void DestroyingMesh();

	UPROPERTY()
		FTimerHandle TimerForDisappear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Destructible)
		class UBoxComponent* TriggerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Destructible)
		class UDestructibleComponent* DestructibleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		bool IsTriggerEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		bool IsDestroied;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		float DefaultDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
		float DefaultImpulse;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
