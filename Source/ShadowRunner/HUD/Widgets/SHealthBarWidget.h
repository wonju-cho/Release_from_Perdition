// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
        FProgressBarStyle progressBarStyle;
		FSlateBrush progressBGBrush;
		FSlateBrush fillBrush;
		FSlateBrush bgBrush;
		FSlateBrush strokeBrush;

		void InitializeTextures();

	private:
		mutable float progressDisplayHP;
		float progressTargetHP;
		TAttribute<float> maxHP;

		FLinearColor GetFillColor(float percent);

	protected:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};
