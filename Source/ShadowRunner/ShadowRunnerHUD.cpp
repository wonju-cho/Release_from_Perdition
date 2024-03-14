// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShadowRunnerHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "HealthBarWidget.h"
#include "AmmoWidget.h"
#include "ShadowCooldownWidget.h"
#include "ShadowSpawnCooldownWidget.h"
#include "AbilitiesWidget.h"
#include "LifeSystemWidget.h"
#include "WaveSystemWidget.h"
#include "ShadowRunnerCharacter.h"
#include "TimerWidget.h"
#include "PlayerOnHitWidget.h"
#include "PlayerLowHealthWidget.h"
#include "LockUnlockWidget.h"

AShadowRunnerHUD::AShadowRunnerHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}


void AShadowRunnerHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition(Center.X, Center.Y);

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

void AShadowRunnerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidgetClass)
	{
		healthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarWidgetClass);
		if (healthBarWidget)
		{
			healthBarWidget->AddToViewport();
		}
	}

	if (AmmoWidgetClass)
	{
		ammoWidget = CreateWidget<UAmmoWidget>(GetWorld(), AmmoWidgetClass);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in shadowHUD")));
		if (ammoWidget)
		{
			ammoWidget->AddToViewport();
		}
	}

	if (ShadowCooldownWidgetClass)
	{
		shadowCooldownWidget = CreateWidget<UShadowCooldownWidget>(GetWorld(), ShadowCooldownWidgetClass);
		if (shadowCooldownWidget)
		{
			shadowCooldownWidget->AddToViewport();
		}
	}

	if (ShadowSpawnCooldownWidgetClass)
	{
		shadowSpawnCooldownWidget = CreateWidget<UShadowSpawnCooldownWidget>(GetWorld(), ShadowSpawnCooldownWidgetClass);
		if (shadowSpawnCooldownWidget)
		{
			shadowSpawnCooldownWidget->AddToViewport();
		}
	}

	if (AbilitiesWidgetClass)
	{
		abilitiesWidget = CreateWidget<UAbilitiesWidget>(GetWorld(), AbilitiesWidgetClass);
		if (abilitiesWidget)
		{
			abilitiesWidget->AddToViewport();
		}
	}

	if (TimerWidgetClass)
	{
		timerWidget = CreateWidget<UTimerWidget>(GetWorld(), TimerWidgetClass);
		if (timerWidget)
		{
			timerWidget->AddToViewport();
		}
	}

	if (LockUnlockWidgetClass)
	{
		lockUnlockWidget = CreateWidget<ULockUnlockWidget>(GetWorld(), LockUnlockWidgetClass);
		if (lockUnlockWidget)
		{
			lockUnlockWidget->AddToViewport();
		}
	}

	if (LifeSystemWidgetClass)
	{
		LifeSystemWidget = CreateWidget<ULifeSystemWidget>(GetWorld(), LifeSystemWidgetClass);
		if (LifeSystemWidget)
		{
			LifeSystemWidget->AddToViewport();
		}
	}

	if (WaveSystemWidgetClass)
	{
		WaveSystemWidget = CreateWidget<UWaveSystemWidget>(GetWorld(), WaveSystemWidgetClass);
		if (WaveSystemWidget)
		{
			WaveSystemWidget->AddToViewport();
		}
	}

	if (PlayerOnHitWidgetClass)
	{
		PlayerOnHitWidget = Cast<UPlayerOnHitWidget>(CreateWidget<UPlayerOnHitWidget>(GetWorld(), PlayerOnHitWidgetClass));
		if (PlayerOnHitWidget)
		{
			PlayerOnHitWidget->AddToViewport();
		}
	}
  
  if(PlayerLowHealthWidgetClass)
  {
    PlayerLowHealthWidget = CreateWidget<UPlayerLowHealthWidget>(GetWorld(), PlayerLowHealthWidgetClass);
    if(PlayerLowHealthWidget)
    {
      PlayerLowHealthWidget->AddToViewport();
    }
  }
}

void AShadowRunnerHUD::UpdateHealth(float health, float defaultHealth)
{
	if (healthBarWidget)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in shadowHUD, health is %f"), health));
		healthBarWidget->UpdateHealth(health, defaultHealth);
	}
}

//void AShadowRunnerHUD::RespawnHealth(float health, float defaultHealth)
//{
//	healthBarWidget->UpdateHealth(health, defaultHealth);
//}

void AShadowRunnerHUD::UpdateAmmo(int equipped, int unequipped, int defaultAmmo, int currentWeapon)
{
	if (ammoWidget)
	{
		
		ammoWidget->UpdateAmmo(equipped, unequipped, defaultAmmo, currentWeapon);
	}
}

void AShadowRunnerHUD::UpdateShadowCooldown(float currentTimer, float cooldownTime)
{
	if (shadowCooldownWidget)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in shadowHUD, health is %f"), health));
		shadowCooldownWidget->UpdateShadowCooldown(currentTimer, cooldownTime);
	}
}

void AShadowRunnerHUD::UpdateShadowSpawnCooldown(float currentTimer, float cooldownTime)
{
	if (shadowSpawnCooldownWidget)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in shadowHUD, health is %f"), health));
		shadowSpawnCooldownWidget->UpdateShadowSpawnCooldown(currentTimer, cooldownTime);
	}
}

void AShadowRunnerHUD::UpdateAbilities(AShadowRunnerCharacter* player)
{
	if (AbilitiesWidgetClass)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in shadowHUD, health is %f"), health));
		abilitiesWidget->UpdateAbilities(player);
	}
}

void AShadowRunnerHUD::UpdateTimer(int32 hour, int32 min, float sec)
{
	if (TimerWidgetClass)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in shadowHUD, health is %f"), health));
		timerWidget->UpdateTimer(hour, min, sec);
	}
}

void AShadowRunnerHUD::DisplayLocked()
{
	if (LockUnlockWidgetClass)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in shadowHUD, health is %f"), health));
		lockUnlockWidget->DisplayLocked();
	}
}

void AShadowRunnerHUD::DisplayUnlocked()
{
	if (LockUnlockWidgetClass)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in shadowHUD, health is %f"), health));
		lockUnlockWidget->DisplayUnlocked();
	}
}

void AShadowRunnerHUD::UpdateLifeSystem(int lifes)
{
	if (LifeSystemWidgetClass)
	{
		LifeSystemWidget->UpdateLifeSystem(lifes);
	}
}

void AShadowRunnerHUD::UpdateWaveSystem(int waves)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("In HUD")));

	if (WaveSystemWidgetClass)
	{
		WaveSystemWidget->UpdateWaveSystem(waves);
	}
}

void AShadowRunnerHUD::UpdatePlayerOnHitEffect(bool justgothit, float deltatime)
{
	if (PlayerOnHitWidgetClass)
	{
		PlayerOnHitWidget->UpdatePlayerOnHitWidget(justgothit, deltatime);
	}
}

void AShadowRunnerHUD::UpdatePlayerLowHealth(bool low)
{
  if(PlayerLowHealthWidgetClass)
  {
    PlayerLowHealthWidget->UpdatePlayerLowHealth(low);
  }
}