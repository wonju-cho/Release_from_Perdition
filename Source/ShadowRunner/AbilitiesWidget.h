// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitiesWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UAbilitiesWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UAbilitiesWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateAbilities(class AShadowRunnerCharacter* player);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//// Dash text.
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//class UBorder* dashBorder;
	//
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//class UTextBlock* dashText;
	//
	//// Double jump text.
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//class UBorder* doubleJumpBorder;
	//
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//class UTextBlock* doubleJumpText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* BG_Shadow_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* BG_Shadow_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* BG_Dash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Button_Dash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icons_Dash;
};
