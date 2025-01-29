// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ShadowRunnerSlateHUD.h"

// Copyright Epic Games, Inc. All Rights Reserved.
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "HealthComponent.h"
#include "LockUnlockWidget.h"
#include "PlayerOnHitWidget.h"
#include "ShadowrunnerController.h"
#include "PlayerLowHealthWidget.h"
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

	//slate widget 초기화
	if(GEngine && GEngine->GameViewport)
	{
		SAssignNew(AmmoWidget, SAmmoWidget)
		.equippedAmmo(10)
		.unequippedAmmo(INFINITE);

		//ammo widget 추가
		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget).PossiblyNullContent(AmmoWidget.ToSharedRef()));

		SetHealthBarTimerInitialization();
	}

	InitializeUMGWidgets();
}

void AShadowRunnerSlateHUD::InitializeHealthBarWidget ()
{
	AShadowrunnerController* playerController = Cast<AShadowrunnerController>(GetWorld()->GetFirstPlayerController());
	if(playerController)
	{
		AShadowRunnerCharacter* player = Cast<AShadowRunnerCharacter>(playerController->GetPawn());
		if(player && player->GetHealthBar())
		{
			SAssignNew(HealthBarWidget, SHealthBarWidget)
			.maxHP(player->GetHealthBar()->GetDefaultHealth());

			UE_LOG(LogTemp, Warning, TEXT("HealthBar Widget is created."))
			//health bar widget 추가
			GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget).PossiblyNullContent(HealthBarWidget.ToSharedRef()));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player or HealthBar is not ready yet."));
			SetHealthBarTimerInitialization();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is not ready yet."));
		SetHealthBarTimerInitialization();
	}
}

void AShadowRunnerSlateHUD::InitializeUMGWidgets ()
{
	if (LockUnlockWidgetClass)
	{
		LockUnlockWidget = CreateWidget<ULockUnlockWidget>(GetWorld(), LockUnlockWidgetClass);
		if (LockUnlockWidget)
		{
			LockUnlockWidget->AddToViewport();
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

void AShadowRunnerSlateHUD::SetHealthBarTimerInitialization ()
{
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		InitializeHealthBarWidget();
	});
}

void AShadowRunnerSlateHUD::UpdateHealth(float currHP, float targetHP)
{
	if(HealthBarWidget.IsValid())
	{
		HealthBarWidget->UpdateHealth(currHP, targetHP);
	}
}

void AShadowRunnerSlateHUD::UpdateAmmo (int32 equipped, int32 unequipped, int32 defaultAmmo, int32 currentWeapon)
{
	if(AmmoWidget.IsValid())
	{
		AmmoWidget->UpdateAmmo(equipped, unequipped, defaultAmmo, currentWeapon);
	}
}

void AShadowRunnerSlateHUD::UpdateAbilities(AShadowRunnerCharacter* player)
{
	
}

void AShadowRunnerSlateHUD::DisplayLocked()
{
	if (LockUnlockWidgetClass)
	{
		LockUnlockWidget->DisplayLocked();
	}
}

void AShadowRunnerSlateHUD::DisplayUnlocked()
{
	if (LockUnlockWidgetClass)
	{
		LockUnlockWidget->DisplayUnlocked();
	}
}

void AShadowRunnerSlateHUD::UpdatePlayerOnHitEffect(bool justgothit, float deltatime)
{
	if (PlayerOnHitWidgetClass)
	{
		PlayerOnHitWidget->UpdatePlayerOnHitWidget(justgothit, deltatime);
	}
}

void AShadowRunnerSlateHUD::UpdatePlayerLowHealth(bool low)
{
	if(PlayerLowHealthWidgetClass)
	{
		PlayerLowHealthWidget->UpdatePlayerLowHealth(low);
	}
}

