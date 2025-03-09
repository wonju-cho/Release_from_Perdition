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
			SLATE_ATTRIBUTE(bool, BIsShadowActive)
		SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct (const FArguments& InArgs);
	
	void UpdateAbilities(AShadowRunnerCharacter* player);
	
	private:
	TAttribute<bool> BIsShadowActive;

	TSharedPtr<SImage> DashBgImage;
	TSharedPtr<SImage> DashButtonImage;
	TSharedPtr<SImage> DashIconImage;

	EVisibility GetShadowLeftBgVisibility() const;
	EVisibility GetShadowRightBgVisibility() const;

};
