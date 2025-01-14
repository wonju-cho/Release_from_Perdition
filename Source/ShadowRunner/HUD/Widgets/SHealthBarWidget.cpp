// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthBarWidget.h"

#include "SlateOptMacros.h"
#include "Brushes/SlateImageBrush.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SConstraintCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SHealthBarWidget::Construct (const FArguments& InArgs)
{
	
	InitializeTextures();

	progressBarStyle = FProgressBarStyle()
			.SetBackgroundImage(FSlateNoResource())
			.SetFillImage(fillBrush) 
			.SetMarqueeImage(FSlateNoResource());
	
	maxHP = InArgs._maxHP;
	progressTargetHP =  maxHP.Get();

	shakeAnimation = FCurveSequence();
	//0초 지연, 0.5초 지속, 부드럽게 진동
	shakeCurve = shakeAnimation.AddCurve(0.0f, 0.5f, ECurveEaseFunction::QuadInOut);

	//progressBarStyle.FillImage.DrawAs = ESlateBrushDrawType::Image;
	
	ChildSlot
	[
		SNew(SConstraintCanvas)
		+SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Alignment(FVector2D(0.0f,0.0f))
		[
			//cross icon image
			SNew(SOverlay)
			+SOverlay::Slot()
			[
				SNew(SImage)
				.Image(&bgBrush)
			]
		]

		+SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f)) // 프로그래스 바 위치 설정 (좌상단 기준)
		.Alignment(FVector2D(0.0f, 0.0f)) // 좌상단 정렬
		.Offset(FMargin(330, 94, 510, 90))
		[
			SAssignNew(healthProgressBar, SProgressBar)
			.Style(&progressBarStyle)
			.BarFillType(EProgressBarFillType::LeftToRight)
			.Percent(progressTargetHP / maxHP.Get())
		]

		//stroke image
		+SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f)) // 좌측 상단 기준
		.Alignment(FVector2D(0.0f, 0.0f)) // 좌측 상단 정렬
		.Offset(FMargin(320, 78, 530, 120)) // ProgressBar 위치와 동일
		[
			SNew(SImage)
			.Image(&strokeBrush)
		]
	];
}

void SHealthBarWidget::UpdateHealth (float currHP, float targetHP)
{
	if (targetHP >= 0 && maxHP.Get() > 0)
	{
		progressTargetHP = targetHP;
		
		const float percent = progressTargetHP / maxHP.Get();
		UE_LOG(LogTemp, Warning, TEXT("퍼센트 값: %f"), percent);
		UE_LOG(LogTemp, Warning, TEXT("지금 체력: %d 전체체력: %d"), progressTargetHP, maxHP.Get());
		
		healthProgressBar->SetFillColorAndOpacity(GetFillColor(percent));
		healthProgressBar->SetPercent(percent);

		//StartShakeAnimation();
		
		//Invalidate(EInvalidateWidgetReason::Paint);
	}
}

FLinearColor SHealthBarWidget::GetFillColor (float percent)
{
	if (percent >= 0.5f)
	{
		return FLinearColor::Green;
	}
	else if (percent >= 0.25f && percent < 0.5f)
	{
		return FLinearColor::Yellow;
	}
	else // health / defaultHealth < 0.25f
	{
		return FLinearColor::Red;
	}
}

void SHealthBarWidget::InitializeTextures ()
{
	const FString progressBGTextPath = TEXT("/Game/Assets/HUD/HP/HUD_HP_BarFill_alpha.HUD_HP_BarFill_alpha");
	UTexture2D* pgBGTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *progressBGTextPath));
	if(pgBGTex)
	{
		fillBrush = FSlateImageBrush(pgBGTex, FVector2D(2000.f, 1140.f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("pgBGTex is not loaded"));
	}
	
	const FString bgTextPath = TEXT("/Game/Assets/HUD/HP/HUD_HP_Cross");
	UTexture2D* bgTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *bgTextPath));
	if(bgTex)
	{
		bgBrush = FSlateImageBrush(bgTex, FVector2D(1920.f, 1080.f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("bgTex is not loaded"));
	}

	const FString strokeTextPath = TEXT("/Game/Assets/HUD/HP/HUD_HP_BarStroke_resize");
	UTexture2D* strokeTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *strokeTextPath));
	if(strokeTex)
	{
		strokeBrush = FSlateImageBrush(strokeTex, FVector2D(1920.f, 1080.f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("strokeTex is not loaded"));
	}

}

void SHealthBarWidget::StartShakeAnimation ()
{
	if(!shakeAnimation.IsPlaying())
	{
		shakeAnimation.Play(this->AsShared());
	}
}

int32 SHealthBarWidget::OnPaint (const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                 const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                 const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// FVector2D shakeOffset = FVector2D::ZeroVector;
	//
	// if(shakeAnimation.IsPlaying())
	// {
	// 	float animationVal = shakeCurve.GetLerp();
	// 	float shakeMagnitude = 5.0f * FMath::Sin(animationVal * 2.0f * PI); // 진폭을 줄임
	// 	shakeOffset = FVector2D(0, shakeMagnitude);
	//
	// 	// 디버깅 로그로 흔들림 오프셋 확인
	// 	UE_LOG(LogTemp, Warning, TEXT("Shake Offset: %s"), *shakeOffset.ToString());
	// }
	//
	// FSlateRenderTransform Transform(shakeOffset);
	// FGeometry transformGeometry = AllottedGeometry.MakeChild(shakeOffset, FVector2D(1.0f, 1.0f));
	//
	//
	return SCompoundWidget::OnPaint(
		Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled
	);
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
