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
	
	maxHP = InArgs._maxHP;
	progressDisplayHP = progressTargetHP =  maxHP.Get();
	
	progressBarStyle = FProgressBarStyle()
	.SetBackgroundImage(FSlateNoResource())
	.SetFillImage(fillBrush);
	
	ChildSlot
	[
		SNew(SConstraintCanvas)
		+SConstraintCanvas::Slot()
		.Anchors(FAnchors(1.0f, 0.0f))
		.Alignment(FVector2D(1.0f, 0.0f))
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
				.Percent(progressDisplayHP / maxHP.Get())
			]

			+SOverlay::Slot()[
				SNew(SImage)
				.Image(&strokeBrush)
			]
		]
	];
	
}

void SHealthBarWidget::UpdateHealth (float currHP, float targetHP)
{
	if (targetHP >= 0 && maxHP.Get() > 0)
	{
		progressDisplayHP = currHP;
		progressTargetHP = targetHP;
		
		//healthProgressBar->SetPercent((progressDisplayHP / maxHP.Get()) * 0.28f);
		const float percent = progressTargetHP / maxHP.Get();
		healthProgressBar->SetFillColorAndOpacity(GetFillColor(percent));

		Invalidate(EInvalidateWidgetReason::Paint);
	}
}

void SHealthBarWidget::InitializeTextures ()
{
	const FString progressBGTextPath = TEXT("/Game/Assets/HUD/HP/HUD_HP_BarFill");
	UTexture2D* pgBGTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *progressBGTextPath));
	if(pgBGTex)
	{
		progressBGBrush = FSlateImageBrush(pgBGTex, FVector2D(1920.f, 1080.f));
		fillBrush = FSlateImageBrush(pgBGTex, FVector2D(1920.f, 1080.f));
	}
	
	const FString bgTextPath = TEXT("/Game/Assets/HUD/HP/HUD_HP_Cross");
	UTexture2D* bgTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *bgTextPath));
	if(bgTex)
	{
		bgBrush = FSlateImageBrush(bgTex, FVector2D(1920.f, 1080.f));
	}

	const FString strokeTextPath = TEXT("/Game/Assets/HUD/HP/HUD_HP_BarStroke.HUD_HP_BarStroke");
	UTexture2D* strokeTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *strokeTextPath));
	if(bgTex)
	{
		strokeBrush = FSlateImageBrush(strokeTex, FVector2D(1920.f, 1080.f));
	}

}

FLinearColor SHealthBarWidget::GetFillColor (float percent)
{
	if (percent >= 0.5f)
	{
		return FLinearColor::Green;
	}
	else if (percent >= 0.25f && percent < 0.5f)
	{
		return FLinearColor::Yellow;
	}
	else // health / defaultHealth < 0.25f
	{
		return FLinearColor::Red;
	}
}

int32 SHealthBarWidget::OnPaint (const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                 const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                 const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	float lerpSpeed = 0.1f;
	
	progressDisplayHP = FMath::Lerp(progressDisplayHP, progressTargetHP, lerpSpeed);
	
	if (healthProgressBar.IsValid())
	{
		healthProgressBar->SetPercent(progressDisplayHP / maxHP.Get() * 0.28f);
	}

	if (!FMath::IsNearlyEqual(progressDisplayHP, progressTargetHP, 0.01f))
	{
		const_cast<SHealthBarWidget*>(this)->Invalidate(EInvalidateWidgetReason::Paint);
	}
	
	return SCompoundWidget::OnPaint(
		Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled
	);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
