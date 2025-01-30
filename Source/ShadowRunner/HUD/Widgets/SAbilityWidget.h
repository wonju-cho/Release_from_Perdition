// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

class AShadowRunnerCharacter;

class SHADOWRUNNER_API SAbilityWidget: public SCompoundWidget
{
	public:
		SLATE_BEGIN_ARGS(SAbilityWidget) {}
			SLATE_ATTRIBUTE(bool, bIsShadowActive)
		SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct (const FArguments& InArgs);
	
	void UpdateAbilities(AShadowRunnerCharacter* player);
	
	private:
	TAttribute<bool> bIsShadowActive;

	TSharedPtr<SImage> dashBGImage;
	TSharedPtr<SImage> dashButtonImage;
	TSharedPtr<SImage> dashIconImage;

	EVisibility GetShadowLeftBGVisibility() const;
	EVisibility GetShadowRightBGVisibility() const;
	
	struct FAbilityTextureSet
	{
		FSlateBrush shadowButtonsBrush;
		FSlateBrush shadowIconsBrush;
		FSlateBrush shadowBGBrush;
		FSlateBrush shadowLeftBGBrush;
		FSlateBrush shadowRightBGBrush;
		FSlateBrush dashButtonBrush;
		FSlateBrush dashBGBrush;
		FSlateBrush dashIconBrush;
	};
		
	FAbilityTextureSet AbilityTextures;
		
	void InitializeTextures();
};
