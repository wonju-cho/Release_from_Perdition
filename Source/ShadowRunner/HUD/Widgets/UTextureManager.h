// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UTextureManager.generated.h"

/**
 * 
 */

namespace TextureKeys
{
	static const FName GAmmoBg = "AmmoBG";
	static const FName GRifle = "Rifle";
	static const FName GPistol = "Pistol";
	static const FName GBigAmmo = "BigAmmo";
	static const FName GSmallAmmo = "SmallAmmo";
	static const FName GShadowButton = "ShadowButton";
	static const FName GShadowIcon = "ShadowIcon";
	static const FName GShadowBg = "ShadowBG";
	static const FName GShadowLeftBg = "ShadowLeftBG";
	static const FName GShadowRightBg = "ShadowRightBG";
	static const FName GDashButton = "DashButton";
	static const FName GDashBg = "DashBG";
	static const FName GDashIcon = "DashIcon";
	static const FName GHPProgressBg = "HPProgressBG";
	static const FName GHPCrossIcon = "HPCrossIcon";
	static const FName GHPBarStroke = "HPBarStrokeIcon";
}

UCLASS()
class SHADOWRUNNER_API UUTextureManager: public UObject
{
	GENERATED_BODY()
public:

	UUTextureManager();

	static UUTextureManager* Get();
	
	//텍스쳐 초기화
	void LoadTextures();

	const FSlateBrush* GetBrush(FName BrushName);
	
	UFUNCTION(BlueprintCallable, Category = "TextureManager")
	void ShutDown();

private:

	static UUTextureManager* Instance;
	
	UPROPERTY()
	TMap<FName, UTexture2D*> TextureMap;

	TMap<FName, FSlateBrush> BrushMap;
	
};
