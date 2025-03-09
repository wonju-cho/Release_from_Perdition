// Fill out your copyright notice in the Description page of Project Settings.

#include "SAmmoWidget.h"
#include "SlateOptMacros.h"
#include "UTextureManager.h"
#include "Brushes/SlateImageBrush.h"
#include "Renderer/Private/ScenePrivate.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SConstraintCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAmmoWidget::Construct (const FArguments& InArgs)
{
	EquippedAmmo = InArgs._EquippedAmmo;
	UnequippedAmmo = InArgs._UnequippedAmmo;

	const FString  FontPath   = TEXT("/Game/Assets/Font/UnicaOne-Regular_Font.UnicaOne-Regular_Font");
	const UObject* FontObject = StaticLoadObject(UObject::StaticClass(), nullptr, *FontPath);
	if (!FontObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no font in the asset"));
	}

	const float shearY = 7.0f / 89.0f;

	UUTextureManager* Instance = UUTextureManager::Get();

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
				.Image(Instance->GetBrush(TextureKeys::GAmmoBg))
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
							SAssignNew(EquippedImage, SImage)
							.Image(Instance->GetBrush(TextureKeys::GRifle))
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
							SAssignNew(EquippedAmmoText, STextBlock)
							.Font(FSlateFontInfo(FontObject, 80))
							.Text_Lambda([this] ()
							{
								if (EquippedAmmo.Get() == INFINITE)
								{
									return infiniteText;
								}
								return FText::FromString(FString::Printf(TEXT("%d"), EquippedAmmo.Get()));
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
							SAssignNew(EquippedAmmoImage, SImage)
							.Image(Instance->GetBrush(TextureKeys::GBigAmmo))
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
							SAssignNew(UnequippedImage, SImage)
							.Image(Instance->GetBrush(TextureKeys::GPistol))
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
							SAssignNew(UnequippedAmmoText, STextBlock)
							.Font(FSlateFontInfo(FontObject, 50))
							.Text_Lambda([this] ()
							{
								if(UnequippedAmmo.Get() == INFINITE)
								{
									return infiniteText;
								}
								return FText::FromString(FString::Printf(TEXT("%d"), UnequippedAmmo.Get()));
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
							SAssignNew(UnequippedAmmoImage, SImage)
							.Image(Instance->GetBrush(TextureKeys::GSmallAmmo))
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
		EquippedAmmo.Set(equipped);
		UnequippedAmmo.Set(unequipped);

		UUTextureManager* Instance = UUTextureManager::Get();
		if(currentWeapon == 0) //rifle
		{
			EquippedImage->SetImage(Instance->GetBrush(TextureKeys::GRifle));
			EquippedAmmoImage->SetImage(Instance->GetBrush(TextureKeys::GBigAmmo));
			UnequippedImage->SetImage(Instance->GetBrush(TextureKeys::GPistol));
			UnequippedAmmoImage->SetImage(Instance->GetBrush(TextureKeys::GSmallAmmo));
		}
		else
		{
			EquippedImage->SetImage(Instance->GetBrush(TextureKeys::GPistol));
			EquippedAmmoImage->SetImage(Instance->GetBrush(TextureKeys::GSmallAmmo));
			UnequippedImage->SetImage(Instance->GetBrush(TextureKeys::GRifle));
			UnequippedAmmoImage->SetImage(Instance->GetBrush(TextureKeys::GBigAmmo));
		}
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
