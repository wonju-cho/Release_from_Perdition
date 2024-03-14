// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSystemWidget.h"
//#include "Components/TextBlock.h"
#include "Runtime/UMG/Public/UMG.h"


UWaveSystemWidget::UWaveSystemWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWaveSystemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//WaveCount->SetVisibility(ESlateVisibility::Visible);
}

void UWaveSystemWidget::UpdateWaveSystem(int waves)
{
	//WaveCount;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), waves));
	//if(WaveCount)
	{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), WaveCount));
		WaveCount->SetText(FText::FromString(FString::FromInt(waves)));
	}
}
