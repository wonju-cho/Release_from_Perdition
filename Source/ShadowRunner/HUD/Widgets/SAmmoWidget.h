// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

#define INFINITE 2147483647 
class SHADOWRUNNER_API SAmmoWidget: public SCompoundWidget
{
	public:
		SLATE_BEGIN_ARGS(SAmmoWidget) {}
			SLATE_ATTRIBUTE(int32, equippedAmmo)
			SLATE_ATTRIBUTE(int32, unequippedAmmo)
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct (const FArguments& InArgs);

		void UpdateAmmo(int32 equipped, int32 unequipped, int32 defaultAmmo, int32 currentWeapon);

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

	struct FAmmoTextureSet
	{
		FSlateBrush rifleBrush;
		FSlateBrush pistolBrush;
		FSlateBrush bigAmmoBrush;
		FSlateBrush smallAmmoBrush;
		FSlateBrush bgBrush;
	};
	
	FAmmoTextureSet ammoTextures;
	
	void InitializeTextures();
};
