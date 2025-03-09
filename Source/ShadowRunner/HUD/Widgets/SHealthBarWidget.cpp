// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthBarWidget.h"

#include "SlateOptMacros.h"
#include "UTextureManager.h"
#include "Brushes/SlateImageBrush.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SConstraintCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SHealthBarWidget::Construct (const FArguments& InArgs)
{

	UUTextureManager* Instance = UUTextureManager::Get();
	
	ProgressBarStyle = FProgressBarStyle()
			.SetBackgroundImage(FSlateNoResource())
			.SetFillImage(*Instance->GetBrush(TextureKeys::GHPProgressBg)) 
			.SetMarqueeImage(FSlateNoResource());

	MaxHP = InArgs._MaxHP;
	ProgressTargetHP =  MaxHP.Get();
	CurrPercent = ProgressTargetHP / MaxHP.Get();

	shakeAnimation = FCurveSequence();
	//0초 지연, 0.5초 지속, 부드럽게 진동
	shakeCurve = shakeAnimation.AddCurve(0.0f, 0.5f, ECurveEaseFunction::QuadInOut);

	ChildSlot
	[
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.015f, 0.04f, 0.615f, 0.64f)) // 비율로 위치와 크기 설정
		.Alignment(FVector2D(0.0f, 0.0f)) // 좌상단 정렬
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(Instance->GetBrush(TextureKeys::GHPProgressBg))
			]
		]

		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.1f, 0.052f, 0.3f, 0.145f)) // 비율 기반 ProgressBar 설정
		.Alignment(FVector2D(0.0f, 0.0f))
		[
			SAssignNew(HealthProgressBar, SProgressBar)
			.Style(&ProgressBarStyle)
			.BarFillType(EProgressBarFillType::LeftToRight)
			.Percent(CurrPercent)
			.FillColorAndOpacity(ColorSet.green)
		]

		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.1, 0.05f, 0.3f, 0.15f)) // 비율 기반 Stroke 설정
		.Alignment(FVector2D(0.0f, 0.0f))
		[
			SAssignNew(StrokeImage, SImage)
			.Image(Instance->GetBrush(TextureKeys::GHPBarStroke))
		]
	];

}

void SHealthBarWidget::UpdateHealth (float currHP, float targetHP)
{
	if (targetHP >= 0 && MaxHP.Get() > 0)
	{
		ProgressTargetHP = targetHP;
		
		CurrPercent = ProgressTargetHP / MaxHP.Get();
		// UE_LOG(LogTemp, Warning, TEXT("퍼센트 값: %f"), currPercent);
		
		HealthProgressBar->SetFillColorAndOpacity(GetFillColor(CurrPercent));
		HealthProgressBar->SetPercent(CurrPercent);

		if (HealthProgressBar.IsValid())
		{
			StartShakeAnimation();
			Invalidate(EInvalidateWidgetReason::Paint);
		}
		
	}
}

void SHealthBarWidget::Tick (const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	
	if (shakeAnimation.IsPlaying())
	{
		const float animationVal = shakeCurve.GetLerp();
		const float shakeX = 5.0f * FMath::Sin(animationVal * 6.0f * PI);
		const float shakeY = 3.0f * FMath::Cos(animationVal * 6.0f * PI);
		const float dampening = FMath::InterpEaseOut(1.0f, 0.0f, animationVal, 2.0f);
		CachedShakeOffset = FVector2D(shakeX, shakeY) * dampening;

		const float strokeShakeX = 10.0f * FMath::Sin(animationVal * 6.0f * PI);
		const float strokeShakeY = 8.0f * FMath::Cos(animationVal * 6.0f * PI);
		const float strokeDampening = FMath::InterpEaseOut(1.0f, 0.0f, animationVal, 2.0f);
		CachedStrokeShakeOffset = FVector2D(strokeShakeX, strokeShakeY) * strokeDampening;
	}
	else
	{
		CachedShakeOffset = FVector2D::ZeroVector;
		CachedStrokeShakeOffset = FVector2D::ZeroVector;
	}
    
	// 여기서 각 자식 위젯에 변환 적용
	if (HealthProgressBar.IsValid())
	{
		HealthProgressBar->SetRenderTransform(FSlateRenderTransform(CachedShakeOffset));
	}
	if (StrokeImage.IsValid())
	{
		StrokeImage->SetRenderTransform(FSlateRenderTransform(CachedStrokeShakeOffset));
	}
}

void SHealthBarWidget::StartShakeAnimation ()
{
	if(!shakeAnimation.IsPlaying())
	{
		shakeAnimation.Play(this->AsShared());
	}
}

int32 SHealthBarWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	
	if (!HealthProgressBar.IsValid() || !StrokeImage.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid healthProgressBar or strokeImage!"));
		return LayerId; // 오류가 발생하지 않도록 바로 반환
	}
	
	// 기본 페인팅을 먼저 수행
	LayerId = SCompoundWidget::OnPaint(
		Args,
		AllottedGeometry,
		MyCullingRect,
		OutDrawElements,
		LayerId,
		InWidgetStyle,
		bParentEnabled
	);
	
	return LayerId;
}

FLinearColor SHealthBarWidget::GetFillColor (float percent)
{
	if (percent >= 0.5f)
	{
		return ColorSet.green;
	}
	else if (percent >= 0.25f)
	{
		return ColorSet.yellow;
	}
	else
	{
		return ColorSet.red;
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
