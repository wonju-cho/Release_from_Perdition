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
		FViewport* viewport = GEngine->GameViewport->Viewport;
		if(viewport)
		{
			const FIntPoint viewportSize = viewport->GetSizeXY();
			screenSize = FVector2D(viewportSize.X, viewportSize.Y);	
		}
	}
	
	ChildSlot
	[
	    SNew(SConstraintCanvas)
	    +SConstraintCanvas::Slot()
	    .Anchors(FAnchors(1.0f, 1.0f)) // 우측 하단
	    .Alignment(FVector2D(1.0f, 1.0f))
	    .AutoSize(true)
	    [
	    	//배경화면
	        SNew(SOverlay)
	        +SOverlay::Slot()
	        [
	            SNew(SImage)
	            .Image(&ammoTextures.bgBrush)
	        ]
	        
	        //UI들
	        +SOverlay::Slot()
	        [
	            SNew(SBox)
	            .Padding(FMargin(0.0f, 0.f, 120.f, 60.f))
	            .HAlign(HAlign_Right)
	            .VAlign(VAlign_Bottom)
	            [
	            	//equipped ui row
	                SNew(SVerticalBox)
	                +SVerticalBox::Slot()
	                [
	                    SNew(SHorizontalBox)
	                    +SHorizontalBox::Slot()
	                    .Padding(FMargin(5.0f, 5.0f, 10.0f, 5.0f))// 패딩 조절
	                    [
	                        SAssignNew(equippedImage, SImage)
	                        .Image(&ammoTextures.rifleBrush)
	                    ]
	                    
	                    +SHorizontalBox::Slot()
	                    .Padding(FMargin(5.0f, 5.0f, 10.0f, 5.0f)) // 패딩 조절
	                    [
	                        SAssignNew(equippedAmmoText, STextBlock)
	                        .Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 80))
	                        .Text_Lambda([this]()
	                        {
	                            if(equippedAmmo.Get() == INFINITE)
	                            {
	                                return infiniteText;
	                            }
	                            return FText::FromString(FString::Printf(TEXT("%d"), equippedAmmo.Get()));
	                        })
	                        .ColorAndOpacity(FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f)))
	                    ]
	                    +SHorizontalBox::Slot()
	                    .Padding(FMargin(5.0f)) // 패딩 조절
	                    [
	                        SAssignNew(equippedAmmoImage, SImage)
	                        .Image(&ammoTextures.bigAmmoBrush)
	                    ]
	                ]

	                //unequipped ui row
	                +SVerticalBox::Slot()[
	                    SNew(SHorizontalBox)
	                    +SHorizontalBox::Slot()
	                    .Padding(FMargin(5.0f, 5.0f, 20.0f, 5.0f)) // 패딩 조절
	                    [
	                        SAssignNew(unequippedImage, SImage)
	                        .Image(&ammoTextures.pistolBrush)
	                    ]
	                    +SHorizontalBox::Slot()
	                    .Padding(FMargin(5.0f, 5.0f, 10.0f, 5.0f)) // 패딩 조절
	                    [
	                        SAssignNew(unequippedAmmoText, STextBlock)
	                        .Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 50))
	                        .Text_Lambda([this]()
	                        {
	                            if(unequippedAmmo.Get() == INFINITE)
	                            {
	                                return infiniteText;
	                            }
	                            return FText::FromString(FString::Printf(TEXT("%d"), unequippedAmmo.Get()));
	                        })
	                        .ColorAndOpacity(FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f)))
	                    ]
	                    +SHorizontalBox::Slot()
	                    .Padding(FMargin(5.0f)) // 패딩 조절
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
	if(rifleTex)
	{
		ammoTextures.rifleBrush = FSlateImageBrush(rifleTex, FVector2D(150.f, 50.f));
	}
	
	const FString pistolTextPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_Pistol");
	UTexture2D* pistolTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *pistolTextPath));
	if(pistolTex)
	{
		ammoTextures.pistolBrush = FSlateImageBrush(pistolTex, FVector2D(80.f, 52.f));
	}

	const FString bigAmmoTextPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_BigAmmo");
	UTexture2D* bigAmmoTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *bigAmmoTextPath));
	if(bigAmmoTex)
	{
		ammoTextures.bigAmmoBrush = FSlateImageBrush(bigAmmoTex, FVector2D(48.f, 62.f));
	}

	const FString smallAmmoPath = TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_SmallAmmo");
	UTexture2D* smallAmmoTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *smallAmmoPath));
	if(smallAmmoTex)
	{
		ammoTextures.smallAmmoBrush = FSlateImageBrush(smallAmmoTex, FVector2D(25.f, 41.f));
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
