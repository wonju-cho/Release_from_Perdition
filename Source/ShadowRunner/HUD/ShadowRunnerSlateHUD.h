// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "SAmmoWidget.h"
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
	void UpdateHealth(float health, float defaultHealth);
	//void RespawnHealth(float health, float defaultHealth);

	UFUNCTION()
	void UpdateAmmo(int32 equipped, int32 unequipped, int32 defaultAmmo, int32 currentWeapon);

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
    

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	/*slate ui 변환*/
	TSharedPtr<SAmmoWidget> ammoWidget;
};
