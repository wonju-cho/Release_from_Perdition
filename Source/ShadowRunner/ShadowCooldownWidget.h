// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShadowCooldownWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UShadowCooldownWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UShadowCooldownWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateShadowCooldown(float currentTimer, float cooldownTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* shadowCooldownProgressBar;
};
