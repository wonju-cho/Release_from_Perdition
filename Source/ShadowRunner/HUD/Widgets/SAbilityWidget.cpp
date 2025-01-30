// Fill out your copyright notice in the Description page of Project Settings.

#include "SAbilityWidget.h"

#include "ShadowRunnerCharacter.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateImageBrush.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SScaleBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAbilityWidget::Construct (const FArguments& InArgs)
{
	//바인딩
	bIsShadowActive = InArgs._bIsShadowActive;

	InitializeTextures();

	ChildSlot
	[
	    SNew(SConstraintCanvas)
	    // Shadow UI 배치
	    + SConstraintCanvas::Slot()
	    .Anchors(FAnchors(0.03f, 0.95f)) // 비율 기반 좌측 하단 위치
	    .Alignment(FVector2D(0.0f, 1.0f)) // 좌측 하단 정렬
	    .AutoSize(true)
	    [
	        SNew(SHorizontalBox)
	        +SHorizontalBox::Slot()
	        .AutoWidth()
	        .Padding(FMargin(0.f)) // 간격 조정 가능
	        [
	            SNew(SVerticalBox)
	            +SVerticalBox::Slot()
	            .AutoHeight()
	            [
	                SNew(SOverlay)
	                +SOverlay::Slot()
	                [
	                    SNew(SImage)
	                    .Image(&AbilityTextures.shadowBGBrush)
	                ]

	                +SOverlay::Slot()
	                [
	                    SNew(SImage)
	                    .Image(&AbilityTextures.shadowLeftBGBrush)
	                    .Visibility(this, &SAbilityWidget::GetShadowLeftBGVisibility)
	                ]

	                +SOverlay::Slot()
	                [
	                    SNew(SImage)
	                    .Image(&AbilityTextures.shadowRightBGBrush)
	                    .Visibility(this, &SAbilityWidget::GetShadowRightBGVisibility)
	                ]

	                +SOverlay::Slot()
	                .HAlign(HAlign_Center)
	                .VAlign(VAlign_Center)
	                [
	                    SNew(SScaleBox)
	                    .Stretch(EStretch::ScaleToFit)
	                    [
	                        SNew(SImage)
	                        .Image(&AbilityTextures.shadowIconsBrush)
	                    ]
	                ]
	            ]
	            +SVerticalBox::Slot()
	            .AutoHeight()
	            .Padding(0.f, -20.f, 0.f, 0.f)
	            [
	                SNew(SImage)
	                .Image(&AbilityTextures.shadowButtonsBrush)
	                .RenderTransform(FSlateRenderTransform(FScale2D(0.9f, 0.9f))) // 크기를 50%로 축소
					.RenderTransformPivot(FVector2D(0.5f, 0.5f)) // 중심점을 기준으로 스케일
	            ]
	        ]

	        // Dash UI 배치
	        +SHorizontalBox::Slot()
	        .AutoWidth()
	        .Padding(FMargin(30.f, -20.f, 0.f, 0.f)) // 간격 추가
	        [
	            SNew(SVerticalBox)
	            +SVerticalBox::Slot()
	            .AutoHeight()
	            [
	                SNew(SOverlay)
	                +SOverlay::Slot()
	                [
	                    SAssignNew(dashBGImage, SImage)
	                    .Image(&AbilityTextures.dashBGBrush)
	                    .Visibility(EVisibility::Hidden)
	                ]

	                +SOverlay::Slot()
	                .HAlign(HAlign_Center)
	                .VAlign(VAlign_Center)
	                [
	                    SNew(SScaleBox)
	                    .Stretch(EStretch::ScaleToFit)
	                    [
	                        SAssignNew(dashIconImage, SImage)
	                        .Image(&AbilityTextures.dashIconBrush)
	                        .Visibility(EVisibility::Hidden)
	                    ]
	                ]
	            ]
	            +SVerticalBox::Slot()
	            .AutoHeight()
	            [
	                SAssignNew(dashButtonImage, SImage)
	                .Image(&AbilityTextures.dashButtonBrush)
	                .Visibility(EVisibility::Hidden)
	            ]
	        ]
	    ]
	];

}

void SAbilityWidget::UpdateAbilities (AShadowRunnerCharacter* player)
{
	 UE_LOG(LogTemp, Warning, TEXT("Dash Visibility: %s"), player->bDashGained ? TEXT("Visible") : TEXT("Hidden"));
	if (player->bDashGained == true)
	{
		dashBGImage->SetVisibility(EVisibility::Visible);
		dashButtonImage->SetVisibility(EVisibility::Visible);
		dashIconImage->SetVisibility(EVisibility::Visible);
	}
}

EVisibility SAbilityWidget::GetShadowLeftBGVisibility () const
{
	return bIsShadowActive.Get() ? EVisibility::Hidden: EVisibility::Visible;
}

EVisibility SAbilityWidget::GetShadowRightBGVisibility () const
{
	return bIsShadowActive.Get() ? EVisibility::Visible: EVisibility::Hidden;
}

void SAbilityWidget::InitializeTextures ()
{
	TMap<FString, FSlateBrush*> TextureMap = {
		{TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_Shadow_Buttons"), &AbilityTextures.shadowButtonsBrush},
		{TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_IconsShadow_Crop"), &AbilityTextures.shadowIconsBrush},
		{TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_BGShadow"), &AbilityTextures.shadowBGBrush},
		{TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_BGShadow_Left"), &AbilityTextures.shadowLeftBGBrush},
		{TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_BGShadow_Right"), &AbilityTextures.shadowRightBGBrush},
		{TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_ButtonsDash"), &AbilityTextures.dashButtonBrush},
		{TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_BGDash"), &AbilityTextures.dashBGBrush},
		{TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_IconsDash"), &AbilityTextures.dashIconBrush},
	};

	for(const TPair<FString, FSlateBrush*>& TexturePair : TextureMap)
	{
		const FString& TexturePath = TexturePair.Key;
		FSlateBrush* BrushPtr = TexturePair.Value;

		UTexture2D* LoadedTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));
		if (LoadedTexture)
		{
			FVector2D TextureSize(LoadedTexture->GetSizeX(), LoadedTexture->GetSizeY());
			*BrushPtr = FSlateImageBrush(LoadedTexture, TextureSize);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load texture: %s"), *TexturePath);
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
