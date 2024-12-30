// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ShadowRunnerSlateHUD.h"

// Copyright Epic Games, Inc. All Rights Reserved.
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "../AmmoWidget.h"
#include "../ShadowRunnerCharacter.h"
#include "Widgets/SWeakWidget.h"

AShadowRunnerSlateHUD::AShadowRunnerSlateHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}


void AShadowRunnerSlateHUD::DrawHUD()
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

void AShadowRunnerSlateHUD::BeginPlay()
{
	Super::BeginPlay();

	if(GEngine && GEngine->GameViewport)
	{
		SAssignNew(ammoWidget, SAmmoWidget)
		.equippedAmmo(10)
		.unequippedAmmo(INFINITE);

		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget).PossiblyNullContent(ammoWidget.ToSharedRef()));
	}

}

void AShadowRunnerSlateHUD::UpdateHealth(float health, float defaultHealth)
{

}

void AShadowRunnerSlateHUD::UpdateAmmo (int32 equipped, int32 unequipped, int32 defaultAmmo, int32 currentWeapon)
{
	if(ammoWidget.IsValid())
	{
		ammoWidget->UpdateAmmo(equipped, unequipped, defaultAmmo, currentWeapon);
	}
}

void AShadowRunnerSlateHUD::UpdateShadowCooldown(float currentTimer, float cooldownTime)
{

}

void AShadowRunnerSlateHUD::UpdateShadowSpawnCooldown(float currentTimer, float cooldownTime)
{

}

void AShadowRunnerSlateHUD::UpdateAbilities(AShadowRunnerCharacter* player)
{

}

void AShadowRunnerSlateHUD::UpdateTimer(int32 hour, int32 min, float sec)
{

}

void AShadowRunnerSlateHUD::DisplayLocked()
{

}

void AShadowRunnerSlateHUD::DisplayUnlocked()
{
	
}

void AShadowRunnerSlateHUD::UpdateLifeSystem(int lifes)
{

}

void AShadowRunnerSlateHUD::UpdateWaveSystem(int waves)
{

}

void AShadowRunnerSlateHUD::UpdatePlayerOnHitEffect(bool justgothit, float deltatime)
{

}

void AShadowRunnerSlateHUD::UpdatePlayerLowHealth(bool low)
{

}