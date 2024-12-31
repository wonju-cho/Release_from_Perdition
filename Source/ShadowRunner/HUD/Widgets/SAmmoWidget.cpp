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

	const FString fontPath = TEXT("/Game/Assets/Font/UnicaOne-Regular_Font.UnicaOne-Regular_Font");
	UObject* fontObject = StaticLoadObject(UObject::StaticClass(), nullptr, *fontPath);
	if (!fontObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no font in the asset"));
	}

	const float shearY = 7.0f / 89.0f;

	ChildSlot
	[
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(1.0f, 1.0f)) // 우측 하단
		.Alignment(FVector2D(1.0f, 1.0f))
		.AutoSize(true)
		[
			//배경화면
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(&ammoTextures.bgBrush)
			]

			//equipped uis
			+SOverlay::Slot()
			[
				SNew(SBox)
				.Padding(FMargin(0.0f, 0.f, 60.f, 160.f))
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				.WidthOverride(450.0f)
				.HeightOverride(140.0f)
				[
					SNew(SHorizontalBox)
					//equipped image
					+SHorizontalBox::Slot()
					.Padding(FMargin(10.0f, 0.0f, 10.0f, 0.0f)) 
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(130.f)
						.HeightOverride(50.f)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SAssignNew(equippedImage, SImage)
							.Image(&ammoTextures.rifleBrush)
							
						]
					]
					
					//equipped ammo text
					+SHorizontalBox::Slot()
					.Padding(FMargin(20.0f, 10.0f, 20.0f, 0.0f)) 
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(160.f)
						.HeightOverride(132.f)
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Center)
						[
							SAssignNew(equippedAmmoText, STextBlock)
							.Font(FSlateFontInfo(fontObject, 80))
							.Text_Lambda([this] ()
							{
								if (equippedAmmo.Get() == INFINITE)
								{
									return infiniteText;
								}
								return FText::FromString(FString::Printf(TEXT("%d"), equippedAmmo.Get()));
							})
							.RenderTransform(FSlateRenderTransform(FShear2D(0.0f, shearY))) //기울이기
							.ColorAndOpacity(FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f)))
						]
					]
				
					//equipped ammo image
					+SHorizontalBox::Slot()
					.Padding(FMargin(16.0f, 30.0f, 16.0f, 0.0f)) 
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(48.f)
						.HeightOverride(67.f)
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Center)
						[
							SAssignNew(equippedAmmoImage, SImage)
							.Image(&ammoTextures.bigAmmoBrush)
						]
					]
				]
			]

			//unequipped uis
			+SOverlay::Slot()
			[
				SNew(SBox)
				.Padding(FMargin(0.0f, 0.f, 60.f, 70.f))
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				.WidthOverride(450.0f)
				.HeightOverride(80.0f)
				[
					SNew(SHorizontalBox)
					//unequipped image
					+SHorizontalBox::Slot()
					.Padding(FMargin(35.0f, 0.0f, 35.0f, 0.0f))
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(80.0f)
						.HeightOverride(52.0f)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SAssignNew(unequippedImage, SImage)
							.Image(&ammoTextures.pistolBrush)
						]
					]
					
					//equipped ammo text
					+SHorizontalBox::Slot()
					.Padding(FMargin(20.0f, 30.0f, 30.0f, 0.0f)) 
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(150.f)
						.HeightOverride(72.0f)
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Center)
						[
							SAssignNew(unequippedAmmoText, STextBlock)
							.Font(FSlateFontInfo(fontObject, 50))
							.Text_Lambda([this] ()
							{
								if(unequippedAmmo.Get() == INFINITE)
								{
									return infiniteText;
								}
								return FText::FromString(FString::Printf(TEXT("%d"), unequippedAmmo.Get()));
							})
							.RenderTransform(FSlateRenderTransform(FShear2D(0.0f, shearY))) //기울이기
							.ColorAndOpacity(FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f)))
						]
					]
				
					//unequipped ammo image
					+SHorizontalBox::Slot()
					.Padding(FMargin(27.5f, 50.0f, 27.5f, 0.0f))
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(25.0f)
						.HeightOverride(41.0f)
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Center)
						[
							SAssignNew(unequippedAmmoImage, SImage)
							.Image(&ammoTextures.smallAmmoBrush)
						]
					]
				]
			]
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
		ammoTextures.bgBrush = FSlateImageBrush(bgTex, FVector2D(1920.f, 1080.f));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load rifle texture: %s"), *bgTextPath);
	}

	const FString rifleTextPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_Rifle");
	UTexture2D* rifleTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *rifleTextPath));
	FVector2D textureSize = FVector2D(rifleTex->GetSizeX(), rifleTex->GetSizeY());
	if(rifleTex)
	{
		textureSize = FVector2D(rifleTex->GetSizeX(), rifleTex->GetSizeY());
		ammoTextures.rifleBrush = FSlateImageBrush(rifleTex, textureSize);
	}
	
	const FString pistolTextPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_Pistol");
	UTexture2D* pistolTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *pistolTextPath));
	if(pistolTex)
	{
		textureSize = FVector2D(pistolTex->GetSizeX(), pistolTex->GetSizeY());
		ammoTextures.pistolBrush = FSlateImageBrush(pistolTex, textureSize);
	}

	const FString bigAmmoTextPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_BigAmmo");
	UTexture2D* bigAmmoTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *bigAmmoTextPath));
	if(bigAmmoTex)
	{
		textureSize = FVector2D(bigAmmoTex->GetSizeX(), bigAmmoTex->GetSizeY());
		ammoTextures.bigAmmoBrush = FSlateImageBrush(bigAmmoTex, textureSize);
	}

	const FString smallAmmoPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_SmallAmmo");
	UTexture2D* smallAmmoTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *smallAmmoPath));
	if(smallAmmoTex)
	{
		//textureSize = FVector2D(smallAmmoTex->GetSizeX(), smallAmmoTex->GetSizeY());
		ammoTextures.smallAmmoBrush = FSlateImageBrush(smallAmmoTex, textureSize);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
