// Fill out your copyright notice in the Description page of Project Settings.


#include "LockUnlockWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "ShadowRunnerCharacter.h"
#include "Engine/EngineTypes.h"

ULockUnlockWidget::ULockUnlockWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ULockUnlockWidget::NativeConstruct()
{
  Super::NativeConstruct();

  // Hide the text initially.
  lockedText->SetVisibility(ESlateVisibility::Hidden);
  unlockedText->SetVisibility(ESlateVisibility::Hidden);
}

void ULockUnlockWidget::DisplayLocked()
{
  //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("in TimerWidget: %d:%d:%f"), hour, min, sec));

  lockedText->SetVisibility(ESlateVisibility::Visible);

  if (!GetWorld()->GetTimerManager().IsTimerActive(lockUnlockTimerHandle))
  {
    GetWorld()->GetTimerManager().SetTimer(lockUnlockTimerHandle, this, &ULockUnlockWidget::ResetLockUnlockText, 2.0f, false);
  }
  else
  {
    GetWorld()->GetTimerManager().ClearTimer(lockUnlockTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(lockUnlockTimerHandle, this, &ULockUnlockWidget::ResetLockUnlockText, 2.0f, false);
  }
}

void ULockUnlockWidget::DisplayUnlocked()
{
  unlockedText->SetVisibility(ESlateVisibility::Visible);

  if (!GetWorld()->GetTimerManager().IsTimerActive(lockUnlockTimerHandle))
  {
    GetWorld()->GetTimerManager().SetTimer(lockUnlockTimerHandle, this, &ULockUnlockWidget::ResetLockUnlockText, 2.0f, false);
  }
  else
  {
    GetWorld()->GetTimerManager().ClearTimer(lockUnlockTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(lockUnlockTimerHandle, this, &ULockUnlockWidget::ResetLockUnlockText, 2.0f, false);
  }
}

void ULockUnlockWidget::ResetLockUnlockText()
{
  if (lockedText && unlockedText)
  {
    lockedText->SetVisibility(ESlateVisibility::Hidden);
    unlockedText->SetVisibility(ESlateVisibility::Hidden);
  }
}