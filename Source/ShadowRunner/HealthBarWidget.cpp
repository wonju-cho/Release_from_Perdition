// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBarWidget.h"
#include "Runtime/UMG/Public/UMG.h"

UHealthBarWidget::UHealthBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UHealthBarWidget::NativeConstruct()
{
  Super::NativeConstruct();
}

void UHealthBarWidget::UpdateHealth(float health, float defaultHealth)
{
  if (health >= 0 && defaultHealth > 0)
  {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in healtBarWidget, health is %f"), health));
		//healthProgressBar->Percent = health / defaultHealth; // This one doesn't work. (maybe Percent is read only?)

        // Barak changes: changed scaling to fit in my health bar image
      healthProgressBar->SetPercent((health / defaultHealth) * 0.28f);

    if (health / defaultHealth >= 0.5f)
    {
      healthProgressBar->SetFillColorAndOpacity(FLinearColor::Green);
    }
    else if (health / defaultHealth >= 0.25f && health / defaultHealth < 0.5f)
    {
      healthProgressBar->SetFillColorAndOpacity(FLinearColor::Yellow);
    }
    else // health / defaultHealth < 0.25f
    {
      healthProgressBar->SetFillColorAndOpacity(FLinearColor::Red);
    }

    if (damageShakeAnim)
    {
      PlayAnimation(damageShakeAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
    }
  }
}