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

	virtual void BeginDestroy() override;

	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
	void UpdateAmmo(int32 equipped, int32 unequipped, int32 defaultAmmo, int32 currentWeapon);

	UFUNCTION()
	void UpdateHealth(float currHP, float targetHP);

	UFUNCTION()
	void UpdateShadowCooldown(float currentTimer, float cooldownTime);

	UFUNCTION()
	void UpdateShadowSpawnCooldown(float currentTimer, float cooldownTime);

	UFUNCTION()
	void UpdateAbilities(AShadowRunnerCharacter* player);

	// UFUNCTION()
	// void UpdateTimer(int32 hour, int32 min, float sec);

	UFUNCTION()
	void DisplayLocked();

	UFUNCTION()
	void DisplayUnlocked();

	// UFUNCTION()
	// 	void UpdateLifeSystem(int lifes);
	//
	UFUNCTION()
		void UpdateWaveSystem(int waves);

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
	
	// UPROPERTY(EditAnywhere, Category = "Widgets")
	// TSubclassOf<UUserWidget> TimerWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> WaveSystemWidgetClass;
private:
	/** Crosshair asset pointer */
	UPROPERTY()
	UTexture2D* CrosshairTex;
	
	int32 HealthBarRetryCount = 0; // 최대 재시도 횟수 추적
	const int32 MaxHealthBarRetries = 5; // 최대 5번 시도 후 중단

	FTimerHandle HealthBarTimerHandle;
	
	/*slate ui들*/
	TSharedPtr<SAmmoWidget> AmmoWidget;
	TSharedPtr<SHealthBarWidget> HealthBarWidget;
	TSharedPtr<SAbilityWidget> AbilityWidget;

	bool bIsAmmoWidgetAdded = false;
	bool bIsAbilityWidgetAdded = false;

	UPROPERTY()
	class ULockUnlockWidget* LockUnlockWidget;

	UPROPERTY()
	class UPlayerOnHitWidget* PlayerOnHitWidget;

	UPROPERTY()
	class UPlayerLowHealthWidget* PlayerLowHealthWidget;

	UPROPERTY()
	class UShadowCooldownWidget* ShadowCooldownWidget;

	UPROPERTY()
	class UShadowSpawnCooldownWidget* ShadowSpawnCooldownWidget;

	//class UTimerWidget* TimerWidget;

	UPROPERTY()
	class UWaveSystemWidget* WaveSystemWidget;
	
	void InitializeHealthBarWidget();

	void InitializeUMGWidgets();
	
	void SetHealthBarTimerInitialization();
};
