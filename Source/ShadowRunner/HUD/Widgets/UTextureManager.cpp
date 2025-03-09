// Fill out your copyright notice in the Description page of Project Settings.

#include "UTextureManager.h"

#include "Brushes/SlateImageBrush.h"

UUTextureManager* UUTextureManager::Instance = nullptr;

UUTextureManager::UUTextureManager ()
{
}

UUTextureManager* UUTextureManager::Get ()
{
	if(!Instance)
	{
		Instance = NewObject<UUTextureManager>();
		Instance->AddToRoot();
		Instance->LoadTextures();
	}
	return Instance;
}

void UUTextureManager::LoadTextures ()
{
	static const TMap<FName, FString> TexturePaths = {
		{TextureKeys::GAmmoBg, TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_BG")},
		{TextureKeys::GRifle, TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_Rifle")},
		{TextureKeys::GPistol, TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_Pistol")},
		{TextureKeys::GBigAmmo, TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_BigAmmo")},
		{TextureKeys::GSmallAmmo, TEXT("/Game/Assets/HUD/Weapons/HUD_Weapons_SmallAmmo")},
		{TextureKeys::GShadowButton, TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_Shadow_Buttons")},
		{TextureKeys::GShadowIcon, TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_IconsShadow_Crop")},
		{TextureKeys::GShadowBg, TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_BGShadow")},
		{TextureKeys::GShadowLeftBg, TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_BGShadow_Left")},
		{TextureKeys::GShadowRightBg, TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_BGShadow_Right")},
		{TextureKeys::GDashButton, TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_ButtonsDash")},
		{TextureKeys::GDashBg, TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_BGDash")},
		{TextureKeys::GDashIcon, TEXT("/Game/Assets/HUD/Abilities/CropImage/HUD_Abilities_IconsDash")},
		{TextureKeys::GHPProgressBg, TEXT("/Game/Assets/HUD/HP/HUD_HP_BarFill_alpha_white2")},
		{TextureKeys::GHPCrossIcon, TEXT("/Game/Assets/HUD/HP/HUD_HP_Cross")},
		{TextureKeys::GHPBarStroke, TEXT("/Game/Assets/HUD/HP/HUD_HP_BarStroke_resize_alpha")},
	};

	for(auto & Entry: TexturePaths)
	{
		UTexture2D* LoadedTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Entry.Value));

		if(LoadedTexture)
		{
			TextureMap.Add(Entry.Key, LoadedTexture);
			BrushMap.Add(Entry.Key, FSlateImageBrush(LoadedTexture, FVector2D(LoadedTexture->GetSizeX(), LoadedTexture->GetSizeY())));
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Failed to load textures: %s in UTextureManager"), *Entry.Value);
		}
	}
}

const FSlateBrush* UUTextureManager::GetBrush (FName BrushName)
{
	return BrushMap.Contains(BrushName) ? &BrushMap[BrushName] : nullptr;
}

void UUTextureManager::ShutDown ()
{
	if(!Instance) return;
	
	//리소스 해제
	TextureMap.Empty();
	BrushMap.Empty();

	//싱글톤 제거
	if(Instance)
	{
		Instance->RemoveFromRoot();
		Instance = nullptr;
	}
}


