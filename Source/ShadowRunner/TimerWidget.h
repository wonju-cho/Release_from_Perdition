// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTimerWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateTimer(int32 hour, int32 min, float sec);

	// Hour text.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* hourText;

	// Minute text.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* minText;

	// Second text.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* secText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	FString hourString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	FString minString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	FString secString;
};
