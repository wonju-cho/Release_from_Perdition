// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

#define INFINITE 2147483647

class SHADOWRUNNER_API SAmmoWidget: public SCompoundWidget
{
	public:
		SLATE_BEGIN_ARGS(SAmmoWidget) {}
			SLATE_ATTRIBUTE(int32, EquippedAmmo)
			SLATE_ATTRIBUTE(int32, UnequippedAmmo)
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct (const FArguments& InArgs);

		void UpdateAmmo(int32 equipped, int32 unequipped, int32 defaultAmmo, int32 currentWeapon);

	private:
		TAttribute<int32> EquippedAmmo;
		TAttribute<int32> UnequippedAmmo;

		TSharedPtr<STextBlock> EquippedAmmoText;
		TSharedPtr<STextBlock> UnequippedAmmoText;
		TSharedPtr<SImage> EquippedImage;
		TSharedPtr<SImage> UnequippedImage;
		TSharedPtr<SImage> EquippedAmmoImage;
		TSharedPtr<SImage> UnequippedAmmoImage;
		
		FText infiniteText = FText::FromString(TEXT("∞"));
};
