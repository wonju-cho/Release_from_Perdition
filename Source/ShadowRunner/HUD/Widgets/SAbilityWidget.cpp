// Fill out your copyright notice in the Description page of Project Settings.

#include "SAbilityWidget.h"

#include "ShadowRunnerCharacter.h"
#include "SlateOptMacros.h"
#include "UTextureManager.h"
#include "Brushes/SlateImageBrush.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SScaleBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAbilityWidget::Construct (const FArguments& InArgs)
{
	//바인딩
	BIsShadowActive = InArgs._BIsShadowActive;

	UUTextureManager* Instance = UUTextureManager::Get();

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
	                    .Image(Instance->GetBrush(TextureKeys::GShadowBg))
	                ]

	                +SOverlay::Slot()
	                [
	                    SNew(SImage)
	                    .Image(Instance->GetBrush(TextureKeys::GShadowLeftBg))
	                    .Visibility(this, &SAbilityWidget::GetShadowLeftBgVisibility)
	                ]

	                +SOverlay::Slot()
	                [
	                    SNew(SImage)
	                    .Image(Instance->GetBrush(TextureKeys::GShadowRightBg))
	                    .Visibility(this, &SAbilityWidget::GetShadowRightBgVisibility)
	                ]

	                +SOverlay::Slot()
	                .HAlign(HAlign_Center)
	                .VAlign(VAlign_Center)
	                [
	                    SNew(SScaleBox)
	                    .Stretch(EStretch::ScaleToFit)
	                    [
	                        SNew(SImage)
	                        .Image(Instance->GetBrush(TextureKeys::GShadowIcon))
	                    ]
	                ]
	            ]
	            +SVerticalBox::Slot()
	            .AutoHeight()
	            .Padding(0.f, -20.f, 0.f, 0.f)
	            [
	                SNew(SImage)
	                .Image(Instance->GetBrush(TextureKeys::GShadowButton))
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
	                    SAssignNew(DashBgImage, SImage)
	                    .Image(Instance->GetBrush(TextureKeys::GDashBg))
	                    .Visibility(EVisibility::Hidden)
	                ]

	                +SOverlay::Slot()
	                .HAlign(HAlign_Center)
	                .VAlign(VAlign_Center)
	                [
	                    SNew(SScaleBox)
	                    .Stretch(EStretch::ScaleToFit)
	                    [
	                        SAssignNew(DashIconImage, SImage)
	                        .Image(Instance->GetBrush(TextureKeys::GDashIcon))
	                        .Visibility(EVisibility::Hidden)
	                    ]
	                ]
	            ]
	            +SVerticalBox::Slot()
	            .AutoHeight()
	            [
	                SAssignNew(DashButtonImage, SImage)
	                .Image(Instance->GetBrush(TextureKeys::GDashButton))
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
		DashBgImage->SetVisibility(EVisibility::Visible);
		DashButtonImage->SetVisibility(EVisibility::Visible);
		DashIconImage->SetVisibility(EVisibility::Visible);
	}
}

EVisibility SAbilityWidget::GetShadowLeftBgVisibility () const
{
	return BIsShadowActive.Get() ? EVisibility::Hidden: EVisibility::Visible;
}

EVisibility SAbilityWidget::GetShadowRightBgVisibility () const
{
	return BIsShadowActive.Get() ? EVisibility::Visible: EVisibility::Hidden;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
