#include "FUIManager.h"

float FUIManager::GetDPIScale ()
{
	if(!bIsInitialized)
	{
		Initialize();
	}
	return DPIScale;
}

void FUIManager::Initialize ()
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	//DPI 스케일 값 계산
	if(GEngine && GEngine->GameViewport)
	{
		const float BaseWidth = 1920.0f;
		const float BaseHeight = 1080.0f;

		float ScaleX = ViewportSize.X / BaseWidth;
		float ScaleY = ViewportSize.Y / BaseHeight;

		DPIScale = FMath::Min(ScaleX, ScaleY);
	}

	// Safe Zone Padding 계산
	if (FSlateApplication::IsInitialized())
	{
		
	}
		
}
