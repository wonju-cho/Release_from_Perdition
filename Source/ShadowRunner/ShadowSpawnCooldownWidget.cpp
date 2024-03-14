// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowSpawnCooldownWidget.h"
#include "Runtime/UMG/Public/UMG.h"

UShadowSpawnCooldownWidget::UShadowSpawnCooldownWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
  
}

void UShadowSpawnCooldownWidget::NativeConstruct()
{
  Super::NativeConstruct();
  shadowSpawnCooldownProgressBar->SetVisibility(ESlateVisibility::Hidden);
}

void UShadowSpawnCooldownWidget::UpdateShadowSpawnCooldown(float currentTimer, float cooldownTime)
{
  if (cooldownTime > 0.0f)
  {
    if (currentTimer >= 0.0f)
    {
      //shadowSpawnCooldownProgressBar->SetVisibility(ESlateVisibility::Visible); // Set to always hidden until we decide on the replacement.
    }
    else
    {
      shadowSpawnCooldownProgressBar->SetVisibility(ESlateVisibility::Hidden);
      currentTimer = 0.0f;
    }

    //healthProgressBar->Percent = health / defaultHealth; // This one doesn't work. (maybe Percent is read only?)
    shadowSpawnCooldownProgressBar->SetPercent(currentTimer / cooldownTime);
  }
}