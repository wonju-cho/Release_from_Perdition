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
			SLATE_ARGUMENT(float, maxHP)
		SLATE_END_ARGS()
	
		void Construct (const FArguments& InArgs);

		void UpdateHealth (float currHP, float targetHP);

	private:
		TSharedPtr<SProgressBar> healthProgressBar;
		TSharedPtr<SImage> strokeImage;
        FProgressBarStyle progressBarStyle;
		FSlateBrush progressBGBrush;
		FSlateBrush fillBrush;
		// FSlateMaterialBrush* fillBrush;
		FSlateBrush bgBrush;
		FSlateBrush strokeBrush;

		void InitializeTextures();

	private:
		float progressTargetHP;
		float currPercent;
		TAttribute<float> maxHP;

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
