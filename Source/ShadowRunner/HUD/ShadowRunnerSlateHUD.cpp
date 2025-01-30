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
#include "ShadowCooldownWidget.h"
#include "ShadowSpawnCooldownWidget.h"
#include "TimerWidget.h"
#include "WaveSystemWidget.h"
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
		///////////
		//AmmoWidget
		SAssignNew(AmmoWidget, SAmmoWidget)
		.equippedAmmo(10)
		.unequippedAmmo(INFINITE);

		//ammo widget 추가
		if(AmmoWidget.IsValid())
		{
			GEngine->GameViewport->AddViewportWidgetContent(
				SNew(SWeakWidget).PossiblyNullContent(AmmoWidget.ToSharedRef()));
		}
		///////////

		///////////
		// Ability Widget

		AShadowRunnerCharacter* PlayerCharacter = Cast<AShadowRunnerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		SAssignNew(AbilityWidget, SAbilityWidget)
		.bIsShadowActive(TAttribute<bool>::Create(
			[PlayerCharacter]() -> bool
			{
				TWeakObjectPtr<AShadowRunnerCharacter> WeakPlayerCharacter = PlayerCharacter;
				if (WeakPlayerCharacter.IsValid())
				{
					return WeakPlayerCharacter->GetShadowActive();
				}
				return false; // 안전한 기본값
			}
		));
		
		if (PlayerCharacter)
		{
			PlayerCharacter->GetOnShadowActiveChanged().BindLambda([this](bool bActive)
			{
				if (AbilityWidget.IsValid())
				{
					AbilityWidget->Invalidate(EInvalidateWidgetReason::Layout);
				}
			});
		}

		// Ability Widget 추가
		if (GEngine && GEngine->GameViewport)
		{
			if(AbilityWidget.IsValid())
			{
				GEngine->GameViewport->AddViewportWidgetContent(
					SNew(SWeakWidget).PossiblyNullContent(AbilityWidget.ToSharedRef()));	
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GEngine or GameViewport is not ready!"));
		}
		/////

		SetHealthBarTimerInitialization();
	}

	InitializeUMGWidgets();
}

void AShadowRunnerSlateHUD::BeginDestroy()
{
	Super::BeginDestroy();

	if (GEngine && GEngine->GameViewport)
	{
		if (AmmoWidget.IsValid())
		{
			GEngine->GameViewport->RemoveViewportWidgetContent(AmmoWidget.ToSharedRef());
			AmmoWidget.Reset();
		}

		if (HealthBarWidget.IsValid())
		{
			GEngine->GameViewport->RemoveViewportWidgetContent(HealthBarWidget.ToSharedRef());
			HealthBarWidget.Reset();
		}

		if (AbilityWidget.IsValid())
		{
			GEngine->GameViewport->RemoveViewportWidgetContent(AbilityWidget.ToSharedRef());
			AbilityWidget.Reset();
		}
	}
}


void AShadowRunnerSlateHUD::InitializeHealthBarWidget ()
{
	if (!GetWorld()) // GetWorld()가 null이면 실행하지 않음
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() is null in InitializeHealthBarWidget!"));
		return;
	}

	AShadowrunnerController* playerController = Cast<AShadowrunnerController>(GetWorld()->GetFirstPlayerController());
	if (!playerController) // PlayerController가 null이면 실행하지 않음
		{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null!"));
		return;
		}

	AShadowRunnerCharacter* player = Cast<AShadowRunnerCharacter>(playerController->GetPawn());
	if (!player) // PlayerCharacter가 null이면 실행하지 않음
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is null!"));
		return;
	}

	if (!player->GetHealthBar()) // HealthBar가 준비되지 않으면 실행하지 않음
	{
		UE_LOG(LogTemp, Warning, TEXT("Player HealthBar is not ready! Retrying..."));
		SetHealthBarTimerInitialization();
		return;
	}

	// 기존 위젯이 있으면 제거 후 재설정
	if (HealthBarWidget.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(HealthBarWidget.ToSharedRef());
		HealthBarWidget.Reset();
	}

	SAssignNew(HealthBarWidget, SHealthBarWidget)
		.maxHP(player->GetHealthBar()->GetDefaultHealth());

	UE_LOG(LogTemp, Warning, TEXT("HealthBar Widget is created."));

	// HealthBar 위젯 추가
	GEngine->GameViewport->AddViewportWidgetContent(
		SNew(SWeakWidget).PossiblyNullContent(HealthBarWidget.ToSharedRef()));
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

	if (ShadowCooldownWidgetClass)
	{
		ShadowCooldownWidget = CreateWidget<UShadowCooldownWidget>(GetWorld(), ShadowCooldownWidgetClass);
		if (ShadowCooldownWidget)
		{
			ShadowCooldownWidget->AddToViewport();
		}
	}

	if (ShadowSpawnCooldownWidgetClass)
	{
		ShadowSpawnCooldownWidget = CreateWidget<UShadowSpawnCooldownWidget>(GetWorld(), ShadowSpawnCooldownWidgetClass);
		if (ShadowSpawnCooldownWidget)
		{
			ShadowSpawnCooldownWidget->AddToViewport();
		}
	}

	// if (TimerWidgetClass)
	// {
	// 	TimerWidget = CreateWidget<UTimerWidget>(GetWorld(), TimerWidgetClass);
	// 	if (TimerWidget)
	// 	{
	// 		TimerWidget->AddToViewport();
	// 	}
	// }
	
	if (WaveSystemWidgetClass)
	{
		WaveSystemWidget = CreateWidget<UWaveSystemWidget>(GetWorld(), WaveSystemWidgetClass);
		if (WaveSystemWidget)
		{
			WaveSystemWidget->AddToViewport();
		}
	}
}

void AShadowRunnerSlateHUD::SetHealthBarTimerInitialization()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() is null in SetHealthBarTimerInitialization!"));
		return;
	}

	if (HealthBarWidget.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthBarWidget already exists! Skipping initialization."));
		return;
	}

	// 최대 재시도 횟수를 초과하면 중단
	if (HealthBarRetryCount >= MaxHealthBarRetries)
	{
		UE_LOG(LogTemp, Error, TEXT("Max retries reached for HealthBar initialization!"));
		return;
	}

	HealthBarRetryCount++;

	GetWorld()->GetTimerManager().ClearTimer(HealthBarTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		HealthBarTimerHandle,
		[this]() { InitializeHealthBarWidget(); },
		0.1f,
		false
	);
}

void AShadowRunnerSlateHUD::UpdateHealth(float currHP, float targetHP)
{
	if(HealthBarWidget.IsValid())
	{
		HealthBarWidget->UpdateHealth(currHP, targetHP);
	}
}

void AShadowRunnerSlateHUD::UpdateShadowCooldown (float currentTimer, float cooldownTime)
{
	if (ShadowCooldownWidget)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in shadowHUD, health is %f"), health));
		ShadowCooldownWidget->UpdateShadowCooldown(currentTimer, cooldownTime);
	}
}

void AShadowRunnerSlateHUD::UpdateShadowSpawnCooldown (float currentTimer, float cooldownTime)
{
	if (ShadowSpawnCooldownWidget)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in shadowHUD, health is %f"), health));
		ShadowSpawnCooldownWidget->UpdateShadowSpawnCooldown(currentTimer, cooldownTime);
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
	if(AbilityWidget.IsValid())
	{
		AbilityWidget->UpdateAbilities(player);	
	}
}

// void AShadowRunnerSlateHUD::UpdateTimer (int32 hour, int32 min, float sec)
// {
// 	if (TimerWidgetClass)
// 	{
// 		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in shadowHUD, health is %f"), health));
// 		TimerWidget->UpdateTimer(hour, min, sec);
// 	}
// }

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

void AShadowRunnerSlateHUD::UpdateWaveSystem (int waves)
{
	if (WaveSystemWidgetClass)
	{
		WaveSystemWidget->UpdateWaveSystem(waves);
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