// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "ShadowRunnerCharacter.h"

UTimerWidget::UTimerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UTimerWidget::NativeConstruct()
{
  Super::NativeConstruct();
}

void UTimerWidget::UpdateTimer(int32 hour, int32 min, float sec)
{
  //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in TimerWidget: %d:%d:%f"), hour, min, sec));
  hourString = FString::FromInt(hour);
  minString = FString::FromInt(min);
  secString = FString::SanitizeFloat(sec).LeftChop(4);

  // Append 0 in front of the number if it has only one digit.
  if (hourString.Len() < 2)
  {
    hourString = "0" + hourString;
  }

  if (minString.Len() < 2)
  {
    minString = "0" + minString;
  }

  if (secString.Len() > 1) // Hack, should change it later.
  {
    if (secString[1] == '.')
    {
      secString = "0" + secString;
    }
  }

  hourText->SetText(FText::FromString(hourString));
  minText->SetText(FText::FromString(minString));
  secText->SetText(FText::FromString(secString));
}