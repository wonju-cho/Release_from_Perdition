#include "AmmoWidget.h"
#include "Runtime/UMG/Public/UMG.h"

UAmmoWidget::UAmmoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAmmoWidget::UpdateAmmo(int equipped, int unequipped, int defaultAmmo, int currentWeapon)
{
    if (equipped >= 0 && defaultAmmo > 0)
    {
        if (equipped >= INFINITE)
        {
            equippedAmmo->SetText(infCopy->GetText());
            unequippedAmmo->SetText(FText::FromString(FString::FromInt(unequipped)));
        }
        else
        {
            equippedAmmo->SetText(FText::FromString(FString::FromInt(equipped)));
            unequippedAmmo->SetText(infCopy->GetText());
        }

        //Update textures
        FString rifleTexPath = FString("/Game/Assets/HUD/Weapons/HUD_Weapons_Rifle.HUD_Weapons_Rifle");
        UTexture2D* rifleTexObj = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *rifleTexPath));

        FString pistolTexPath = FString("/Game/Assets/HUD/Weapons/HUD_Weapons_Pistol.HUD_Weapons_Pistol");
        UTexture2D* pistolTexObj = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *pistolTexPath));

        FString bigAmmoTexPath = FString("/Game/Assets/HUD/Weapons/HUD_Weapons_BigAmmo.HUD_Weapons_BigAmmo");
        UTexture2D* bigAmmoTexObj = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *bigAmmoTexPath));

        FString smallAmmoTexPath = FString("/Game/Assets/HUD/Weapons/HUD_Weapons_SmallAmmo.HUD_Weapons_SmallAmmo");
        UTexture2D* smallAmmoTexObj = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *smallAmmoTexPath));

        //static ConstructorHelpers::FObjectFinder<UTexture2D> rifleTexObj(TEXT("Texture2D'/Game/Assets/HUD/Weapons/HUD_Weapons_Rifle.HUD_Weapons_Rifle'"));
        //static ConstructorHelpers::FObjectFinder<UTexture2D> pistolTexObj(TEXT("Texture2D'/Game/Assets/HUD/Weapons/HUD_Weapons_Pistol.HUD_Weapons_Pistol'"));
        //static ConstructorHelpers::FObjectFinder<UTexture2D> bigAmmoTexObj(TEXT("Texture2D'/Game/Assets/HUD/Weapons/HUD_Weapons_BigAmmo.HUD_Weapons_BigAmmo'"));
        //static ConstructorHelpers::FObjectFinder<UTexture2D> smallAmmoTexObj(TEXT("Texture2D'/Game/Assets/HUD/Weapons/HUD_Weapons_SmallAmmo.HUD_Weapons_SmallAmmo'"));

        if (currentWeapon == 0)
        {
            equippedTex->SetBrushFromTexture(rifleTexObj);
            equippedTex->SetBrushSize(FVector2D(rifleTexObj->GetSizeX(), rifleTexObj->GetSizeY()));
            unequippedTex->SetBrushFromTexture(pistolTexObj);
            unequippedTex->SetBrushSize(FVector2D(pistolTexObj->GetSizeX(), pistolTexObj->GetSizeY()));
            equippedAmmoTex->SetBrushFromTexture(bigAmmoTexObj);
            equippedAmmoTex->SetBrushSize(FVector2D(bigAmmoTexObj->GetSizeX(), bigAmmoTexObj->GetSizeY()));
            unequippedAmmoTex->SetBrushFromTexture(smallAmmoTexObj);
            unequippedAmmoTex->SetBrushSize(FVector2D(smallAmmoTexObj->GetSizeX(), smallAmmoTexObj->GetSizeY()));
        }
        else
        {
            equippedTex->SetBrushFromTexture(pistolTexObj);
            equippedTex->SetBrushSize(FVector2D(pistolTexObj->GetSizeX(), pistolTexObj->GetSizeY()));
            unequippedTex->SetBrushFromTexture(rifleTexObj);
            unequippedTex->SetBrushSize(FVector2D(rifleTexObj->GetSizeX(), rifleTexObj->GetSizeY()));
            equippedAmmoTex->SetBrushFromTexture(smallAmmoTexObj);
            equippedAmmoTex->SetBrushSize(FVector2D(smallAmmoTexObj->GetSizeX(), smallAmmoTexObj->GetSizeY()));
            unequippedAmmoTex->SetBrushFromTexture(bigAmmoTexObj);
            unequippedAmmoTex->SetBrushSize(FVector2D(bigAmmoTexObj->GetSizeX(), bigAmmoTexObj->GetSizeY()));
        }
    }
}