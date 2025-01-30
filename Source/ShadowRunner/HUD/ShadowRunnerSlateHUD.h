// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "SAbilityWidget.h"
#include "SAmmoWidget.h"
#include "SHealthBarWidget.h"
#include "GameFramework/HUD.h"
#include "ShadowRunnerSlateHUD.generated.h"

UCLASS()
class AShadowRunnerSlateHUD : public AHUD
{
	GENERATED_BODY()

public:
	AShadowRunnerSlateHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateAmmo(int32 equipped, int32 unequipped, int32 defaultAmmo, int32 currentWeapon);

	UFUNCTION()
	void UpdateHealth(float currHP, float targetHP);

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

	// UFUNCTION()
	// 	void UpdateLifeSystem(int lifes);
	//
	// UFUNCTION()
	// 	void UpdateWaveSystem(int waves);

	UFUNCTION()
		void UpdatePlayerOnHitEffect(bool justgothit, float deltatime);

	UFUNCTION()
		void UpdatePlayerLowHealth(bool low);
    
	public:
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> LockUnlockWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerOnHitWidgetClass;
  
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerLowHealthWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> ShadowCooldownWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> ShadowSpawnCooldownWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> TimerWidgetClass;
private:
	/** Crosshair asset pointer */
	UTexture2D* CrosshairTex;

	/*slate ui들*/
	TSharedPtr<SAmmoWidget> AmmoWidget;
	TSharedPtr<SHealthBarWidget> HealthBarWidget;
	TSharedPtr<SAbilityWidget> AbilityWidget;

	class ULockUnlockWidget* LockUnlockWidget;
	class UPlayerOnHitWidget* PlayerOnHitWidget;
	class UPlayerLowHealthWidget* PlayerLowHealthWidget;
	class UShadowCooldownWidget* ShadowCooldownWidget;
	class UShadowSpawnCooldownWidget* ShadowSpawnCooldownWidget;
	class UTimerWidget* TimerWidget;
	
	void InitializeHealthBarWidget();

	void InitializeUMGWidgets();
	
	void SetHealthBarTimerInitialization();
};
