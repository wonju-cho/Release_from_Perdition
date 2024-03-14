// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LifeSystemWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API ULifeSystemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ULifeSystemWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateLifeSystem(int lifes);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* LifeCount;
};
