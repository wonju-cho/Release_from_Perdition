// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHealthBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateHealth(float health, float defaultHealth);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* healthProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* damageShakeAnim;
};
