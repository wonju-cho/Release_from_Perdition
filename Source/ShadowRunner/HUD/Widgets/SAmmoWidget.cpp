// Fill out your copyright notice in the Description page of Project Settings.

#include "SAmmoWidget.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateImageBrush.h"
#include "Renderer/Private/ScenePrivate.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "RHI.h"
#include "RHIResources.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAmmoWidget::Construct (const FArguments& InArgs)
{
	InitializeTextures();
	
	equippedAmmo = InArgs._equippedAmmo;
	unequippedAmmo = InArgs._unequippedAmmo;

	FVector2D screenSize = FVector2D(1920,1080);
	if(GEngine && GEngine -> GameViewport)
	{
		const FIntPoint viewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		screenSize = FVector2D(viewportSize.X, viewportSize.Y);
	}
	
	ChildSlot
	[
		SNew(SConstraintCanvas)

		//위젯 배경화면 텍스쳐
		+SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f,0.5f))
		.Alignment(FVector2D(0.5f,0.5f))
		.Offset(FMargin(0,0,screenSize.X, screenSize.Y))
		[
			SNew(SImage)
			.Image(&ammoTextures.bgBrush)
		]
			
		//equipped Ammo 텍스트
		+SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f,0.5f))
		.Alignment(FVector2D(0.2f,0.3f))
		.Offset(FMargin(screenSize.X * 0.35f, screenSize.Y * 0.27f))
		[
			SAssignNew(equippedAmmoText, STextBlock)
			.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 80)) // 기본 폰트 사용
			.Text_Lambda([this]()
			{
				return FText::FromString(FString::Printf(TEXT("%d"), equippedAmmo.Get()));
			})
		]

		//unequipped Ammo 텍스트
		+SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f,0.5f))
		.Alignment(FVector2D(0.2f,0.3f))
		.Offset(FMargin(screenSize.X * 0.37f, screenSize.Y * 0.39f))
		[
			SAssignNew(unequippedAmmoText, STextBlock)
			.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 50)) // 기본 폰트 사용
			.Text_Lambda([this]()
			{
				return FText::FromString(FString::Printf(TEXT("%d"), unequippedAmmo.Get()));
			})
		]
		
		//equippedAmmo 텍스쳐
		+SConstraintCanvas::Slot()
		[
			SAssignNew(equippedAmmoImage, SImage)
			.Image(FCoreStyle::Get().GetBrush("DefaultBrush"))
		]
		
		//unequippedAmmo 텍스쳐
		+SConstraintCanvas::Slot()
		[
			SAssignNew(unequippedAmmoImage, SImage)
			.Image(FCoreStyle::Get().GetBrush("DefaultBrush"))
		]
	];
	
}

void SAmmoWidget::UpdateAmmo (int32 equipped, int32 unequipped, int32 defaultAmmo, int32 currentWeapon)
{
	if(equipped >= 0 && defaultAmmo > 0)
	{
		equippedAmmo.Set(equipped);
		unequippedAmmo.Set(unequipped);

		if(currentWeapon == 0) //rifle
		{
			equippedImage->SetImage(&ammoTextures.rifleBrush);
			equippedAmmoImage->SetImage(&ammoTextures.bigAmmoBrush);
			unequippedImage->SetImage(&ammoTextures.pistolBrush);
			unequippedAmmoImage->SetImage(&ammoTextures.smallAmmoBrush);
		}
		else
		{
			equippedImage->SetImage(&ammoTextures.pistolBrush);
			equippedAmmoImage->SetImage(&ammoTextures.smallAmmoBrush);
			unequippedImage->SetImage(&ammoTextures.rifleBrush);
			unequippedAmmoImage->SetImage(&ammoTextures.bigAmmoBrush);
		}
	}
}

void SAmmoWidget::InitializeTextures ()
{
	const FString bgTextPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_BG");
	UTexture2D* bgTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *bgTextPath));
	if(bgTex)
	{
		ammoTextures.bgBrush = FSlateImageBrush(bgTex, FVector2D(1920, 1080));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load rifle texture: %s"), *bgTextPath);
	}
	
	const FString rifleTextPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_Rifle");
	UTexture2D* rifleTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *rifleTextPath));
	if(rifleTex)
	{
		ammoTextures.rifleBrush = FSlateImageBrush(rifleTex, FVector2D(100, 40));
	}
	
	const FString pistolTextPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_Pistol");
	UTexture2D* pistolTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *pistolTextPath));
	if(pistolTex)
	{
		ammoTextures.pistolBrush = FSlateImageBrush(pistolTex, FVector2D(100, 40));
	}

	const FString bigAmmoTextPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_BigAmmo");
	UTexture2D* bigAmmoTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *bigAmmoTextPath));
	if(bigAmmoTex)
	{
		ammoTextures.bigAmmoBrush = FSlateImageBrush(bigAmmoTex, FVector2D(100, 40));
	}

	const FString smallAmmoPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_SmallAmmo");
	UTexture2D* smallAmmoTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *smallAmmoPath));
	if(smallAmmoTex)
	{
		ammoTextures.smallAmmoBrush = FSlateImageBrush(smallAmmoTex, FVector2D(100, 40));
	}

}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
