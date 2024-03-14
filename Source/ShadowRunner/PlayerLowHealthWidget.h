// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerLowHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UPlayerLowHealthWidget : public UUserWidget
{
	GENERATED_BODY()
  
  public:
	UPlayerLowHealthWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
  
  void UpdatePlayerLowHealth(bool low);
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
  class UImage* PlayerLowHealthImage;
};
