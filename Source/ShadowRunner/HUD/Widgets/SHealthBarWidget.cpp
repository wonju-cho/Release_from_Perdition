// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthBarWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SHealthBarWidget::Construct (const FArguments& InArgs)
{
	
	ChildSlot
	[
		// Populate the widget
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

int32 SHealthBarWidget::OnPaint (const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
