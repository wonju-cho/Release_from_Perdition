// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthBarWidget.h"

#include "SlateOptMacros.h"
#include "Brushes/SlateImageBrush.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SConstraintCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SHealthBarWidget::Construct (const FArguments& InArgs)
{
	InitializeTextures();

	progressBarStyle = FProgressBarStyle()
	.SetBackgroundImage(progressBGBrush)
	.SetFillImage(fillBrush);
	
	ChildSlot
	[
		SNew(SConstraintCanvas)
		+SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.f, 0.f))
		.Alignment(FVector2D(0.5f, 0.5f))
		.AutoSize(true)
		[
			//배경화면
			SNew(SOverlay)
			+SOverlay::Slot()
			[
				SNew(SImage)
				.Image(&bgBrush)
			]
			
			+SOverlay::Slot()
			[
				SAssignNew(healthProgressBar, SProgressBar)
				.Style(&progressBarStyle)
				.Percent(InArgs._currHP)
			]
			
		]
	
	];
	
}

void SHealthBarWidget::UpdateHealth (float health, float defaultHealth)
{
	if (health >= 0 && defaultHealth > 0)
	{
		healthProgressBar->SetPercent((health / defaultHealth) * 0.28f);

		if (health / defaultHealth >= 0.5f)
		{
			healthProgressBar->SetFillColorAndOpacity(FLinearColor::Green);
		}
		else if (health / defaultHealth >= 0.25f && health / defaultHealth < 0.5f)
		{
			healthProgressBar->SetFillColorAndOpacity(FLinearColor::Yellow);
		}
		else // health / defaultHealth < 0.25f
		{
			healthProgressBar->SetFillColorAndOpacity(FLinearColor::Red);
		}

		Invalidate(EInvalidateWidgetReason::Paint);
	}
}

void SHealthBarWidget::InitializeTextures ()
{
	const FString progressBGTextPath = TEXT("/Game/Assets/HUD/HP/HUD_HP_BarFill.HUD_HP_BarFill");
	UTexture2D* pgBGTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *progressBGTextPath));
	if(pgBGTex)
	{
		progressBGBrush = FSlateImageBrush(pgBGTex, FVector2D(1920.f, 1080.f));
		fillBrush = FSlateImageBrush(pgBGTex, FVector2D(1920.f, 1080.f));
	}
	
	const FString bgTextPath = TEXT("/Game/Assets/HUD/HP/HUD_HP_Cross.HUD_HP_Cross");
	UTexture2D* bgTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *bgTextPath));
	if(bgTex)
	{
		bgBrush = FSlateImageBrush(bgTex, FVector2D(1920.f, 1080.f));
	}

}

int32 SHealthBarWidget::OnPaint (const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                 const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                 const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
