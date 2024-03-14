// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShadowRunnerHUD.generated.h"

UCLASS()
class AShadowRunnerHUD : public AHUD
{
	GENERATED_BODY()

public:
	AShadowRunnerHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateHealth(float health, float defaultHealth);
	//void RespawnHealth(float health, float defaultHealth);

	UFUNCTION()
	void UpdateAmmo(int equipped, int unequipped, int defaultAmmo, int currentWeapon);

	UFUNCTION()
	void UpdateShadowCooldown(float currentTimer, float cooldownTime);

	UFUNCTION()
	void UpdateShadowSpawnCooldown(float currentTimer, float cooldownTime);

	UFUNCTION()
	void UpdateAbilities(class AShadowRunnerCharacter* player);

	UFUNCTION()
	void UpdateTimer(int32 hour, int32 min, float sec);

	UFUNCTION()
	void DisplayLocked();

	UFUNCTION()
	void DisplayUnlocked();

	UFUNCTION()
		void UpdateLifeSystem(int lifes);

	UFUNCTION()
		void UpdateWaveSystem(int waves);

	UFUNCTION()
		void UpdatePlayerOnHitEffect(bool justgothit, float deltatime);

  UFUNCTION()
		void UpdatePlayerLowHealth(bool low);
    
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> AmmoWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> ShadowCooldownWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> ShadowSpawnCooldownWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> AbilitiesWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> TimerWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> LockUnlockWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UUserWidget> LifeSystemWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UUserWidget> WaveSystemWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UUserWidget> PlayerOnHitWidgetClass;
  
  UPROPERTY(EditAnywhere, Category = "Widgets")
    TSubclassOf<UUserWidget> PlayerLowHealthWidgetClass;
    
private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	class UHealthBarWidget* healthBarWidget;
	class UAmmoWidget* ammoWidget;
	class UShadowCooldownWidget* shadowCooldownWidget;
	class UShadowSpawnCooldownWidget* shadowSpawnCooldownWidget;
	class UAbilitiesWidget* abilitiesWidget;
	class UTimerWidget* timerWidget;
	class ULockUnlockWidget* lockUnlockWidget;
	class ULifeSystemWidget* LifeSystemWidget;
	class UWaveSystemWidget* WaveSystemWidget;
	class UPlayerOnHitWidget* PlayerOnHitWidget;
  class UPlayerLowHealthWidget* PlayerLowHealthWidget;
};
