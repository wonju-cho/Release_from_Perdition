// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SHADOWRUNNER_API SAbilityWidget: public SCompoundWidget
{
	public:
		SLATE_BEGIN_ARGS(SAbilityWidget)
			{
			}

		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct (const FArguments& InArgs);

	private:
	TAttribute<int32> equippedAmmo;
	TAttribute<int32> unequippedAmmo;

	TSharedPtr<STextBlock> equippedAmmoText;
	TSharedPtr<STextBlock> unequippedAmmoText;
	TSharedPtr<SImage> equippedImage;
	TSharedPtr<SImage> unequippedImage;
	TSharedPtr<SImage> equippedAmmoImage;
	TSharedPtr<SImage> unequippedAmmoImage;
		
	FText infiniteText = FText::FromString(TEXT("∞"));

	struct FAbilityTextureSet
	{
		FSlateBrush rifleBrush;
		FSlateBrush pistolBrush;
		FSlateBrush bigAmmoBrush;
		FSlateBrush smallAmmoBrush;
		FSlateBrush bgBrush;
	};
		
	FAbilityTextureSet AmmoTextures;
		
	void InitializeTextures();
};
