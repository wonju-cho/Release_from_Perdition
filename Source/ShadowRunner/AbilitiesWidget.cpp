// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"
#include "ShadowRunnerCharacter.h"

UAbilitiesWidget::UAbilitiesWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
  
}

void UAbilitiesWidget::NativeConstruct()
{
  Super::NativeConstruct();

  // Hide the border and the text initially.
  //dashBorder->SetVisibility(ESlateVisibility::Hidden);
  //dashText->SetVisibility(ESlateVisibility::Hidden);
  //doubleJumpBorder->SetVisibility(ESlateVisibility::Hidden);
  //doubleJumpText->SetVisibility(ESlateVisibility::Hidden);
  BG_Dash->SetVisibility(ESlateVisibility::Hidden);
  Button_Dash->SetVisibility(ESlateVisibility::Hidden);
  Icons_Dash->SetVisibility(ESlateVisibility::Hidden);
}

void UAbilitiesWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    AShadowRunnerCharacter* const player_ = Cast<AShadowRunnerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (player_->shadowIsActive == true)
    {
        BG_Shadow_Left->SetVisibility(ESlateVisibility::Hidden);
        BG_Shadow_Right->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        BG_Shadow_Left->SetVisibility(ESlateVisibility::Visible);
        BG_Shadow_Right->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UAbilitiesWidget::UpdateAbilities(AShadowRunnerCharacter* player)
{
  if (player->bDashGained)
  {
    //dashBorder->SetVisibility(ESlateVisibility::Visible);
    //dashText->SetVisibility(ESlateVisibility::Visible);
      BG_Dash->SetVisibility(ESlateVisibility::Visible);
      Button_Dash->SetVisibility(ESlateVisibility::Visible);
      Icons_Dash->SetVisibility(ESlateVisibility::Visible);
  }

  if (player->jumpNumber == 2)
  {
    //doubleJumpBorder->SetVisibility(ESlateVisibility::Visible);
    //doubleJumpText->SetVisibility(ESlateVisibility::Visible);
  }
}