// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeSystemWidget.h"
#include "Runtime/UMG/Public/UMG.h"

ULifeSystemWidget::ULifeSystemWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ULifeSystemWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULifeSystemWidget::UpdateLifeSystem(int lifes)
{
	//LifeCount->SetText(FText::FromString(FString::FromInt(lifes)));
	//LifeCount->SetText(FText::FromString("Infinite"));
}
