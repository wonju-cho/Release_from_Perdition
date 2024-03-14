// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowCooldownWidget.h"
#include "Runtime/UMG/Public/UMG.h"

UShadowCooldownWidget::UShadowCooldownWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
  
}

void UShadowCooldownWidget::NativeConstruct()
{
  Super::NativeConstruct();
  shadowCooldownProgressBar->SetVisibility(ESlateVisibility::Hidden);
}

void UShadowCooldownWidget::UpdateShadowCooldown(float currentTimer, float cooldownTime)
{
  if (cooldownTime > 0.0f)
  {
    if (currentTimer >= 0.0f)
    {
      //shadowCooldownProgressBar->SetVisibility(ESlateVisibility::Visible); // Set to always hidden until we decide on the replacement.
    }
    else
    {
      shadowCooldownProgressBar->SetVisibility(ESlateVisibility::Hidden);
      currentTimer = 0.0f;
    }

    //healthProgressBar->Percent = health / defaultHealth; // This one doesn't work. (maybe Percent is read only?)
    shadowCooldownProgressBar->SetPercent(currentTimer / cooldownTime);
  }
}
