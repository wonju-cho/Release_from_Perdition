// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockUnlockWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API ULockUnlockWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	ULockUnlockWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void DisplayLocked();

	void DisplayUnlocked();

	void ResetLockUnlockText();

	// Timer for displaying locked/unlocked text.
	struct FTimerHandle lockUnlockTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
  class UTextBlock* lockedText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
  class UTextBlock* unlockedText;
};
