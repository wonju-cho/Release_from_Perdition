// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerLowHealthWidget.h"
#include "Runtime/UMG/Public/UMG.h"

UPlayerLowHealthWidget::UPlayerLowHealthWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerLowHealthWidget::NativeConstruct()
{
  Super::NativeConstruct();
  PlayerLowHealthImage->SetVisibility(ESlateVisibility::Visible);
  PlayerLowHealthImage->SetRenderOpacity(0.f);
}

void UPlayerLowHealthWidget::UpdatePlayerLowHealth(bool low)
{
  if(low) PlayerLowHealthImage->SetRenderOpacity(0.3f);
  else PlayerLowHealthImage->SetRenderOpacity(0.f);
}