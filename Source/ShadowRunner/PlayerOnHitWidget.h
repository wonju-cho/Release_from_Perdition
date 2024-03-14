// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOnHitWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UPlayerOnHitWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerOnHitWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdatePlayerOnHitWidget(bool justgothit, float dealtatime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* RedBorderImage;

	UPROPERTY()
	float timer;
};
