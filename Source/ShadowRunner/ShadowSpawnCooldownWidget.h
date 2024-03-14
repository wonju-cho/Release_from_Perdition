// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShadowSpawnCooldownWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UShadowSpawnCooldownWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UShadowSpawnCooldownWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateShadowSpawnCooldown(float currentTimer, float cooldownTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* shadowSpawnCooldownProgressBar;
};
