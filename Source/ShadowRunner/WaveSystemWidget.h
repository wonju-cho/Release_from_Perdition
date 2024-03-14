// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaveSystemWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UWaveSystemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWaveSystemWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateWaveSystem(int waves);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* WaveCount;
};
