// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerOnHitWidget.h"
#include "Runtime/UMG/Public/UMG.h"
//#include "ShadowRunnerHUD.h"

UPlayerOnHitWidget::UPlayerOnHitWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerOnHitWidget::NativeConstruct()
{
	Super::NativeConstruct();
  
	if (RedBorderImage) {
		RedBorderImage->SetVisibility(ESlateVisibility::Visible);
		//RedBorderImage->SetOpacity(0.f); /* hidden */
		RedBorderImage->SetRenderOpacity(0.f);
	}
	timer = 0.f;
}

void UPlayerOnHitWidget::UpdatePlayerOnHitWidget(bool justgothit, float deltatime)
{
	
	if (justgothit == true)
	{
		RedBorderImage->SetRenderOpacity(1);
		timer = 3.f;
		
	}
	else if(timer > 0)
	{
		timer -= deltatime;
		if (IsValid(RedBorderImage)) 
    {
      if(timer < 0.001) timer = 0;
      float newOpa = timer/3;
			RedBorderImage->SetRenderOpacity(newOpa);
		}
		
	}
}
