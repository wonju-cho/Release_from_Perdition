// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateMaterialBrush.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Notifications/SProgressBar.h"

/**
 * 
 */

class SHADOWRUNNER_API SHealthBarWidget: public SCompoundWidget
{
	public:
		SLATE_BEGIN_ARGS(SHealthBarWidget) {}
			SLATE_ARGUMENT(float, MaxHP)
		SLATE_END_ARGS()
	
		void Construct (const FArguments& InArgs);

		void UpdateHealth (float currHP, float targetHP);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	private:
		TSharedPtr<SProgressBar> HealthProgressBar;
		TSharedPtr<SImage> StrokeImage;
		FProgressBarStyle ProgressBarStyle;

	private:
		float ProgressTargetHP;
		float CurrPercent;

		FVector2D CachedShakeOffset;
		FVector2D CachedStrokeShakeOffset;
	
		TAttribute<float> MaxHP;

		FLinearColor GetFillColor(float percent);
		FCurveSequence shakeAnimation;
		FCurveHandle shakeCurve;

		void StartShakeAnimation();

		struct CustomColorSet
		{
			FLinearColor green;
			FLinearColor yellow;
			FLinearColor red;

			CustomColorSet(): green(0.000607f, 0.973445f, 0.006995f, 1.0f),
		  yellow(0.973445f, 0.973445f, 0.006995f, 1.0f),
		  red(0.973445f, 0.000607f, 0.006995f, 1.0f){}
		};

		CustomColorSet ColorSet;
	
	protected:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};
