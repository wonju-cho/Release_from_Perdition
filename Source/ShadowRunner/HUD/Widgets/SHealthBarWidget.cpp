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
	currPercent = progressTargetHP / maxHP.Get();

	shakeAnimation = FCurveSequence();
	//0초 지연, 0.5초 지속, 부드럽게 진동
	shakeCurve = shakeAnimation.AddCurve(0.0f, 0.5f, ECurveEaseFunction::QuadInOut);
	
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
		.Offset(FMargin(300, 72, 485, 75))
		[
			SAssignNew(healthProgressBar, SProgressBar)
			.Style(&progressBarStyle)
			.BarFillType(EProgressBarFillType::LeftToRight)
			.Percent(currPercent)
		]
	
		//stroke image
		+SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f)) // 좌측 상단 기준
		.Alignment(FVector2D(0.0f, 0.0f)) // 좌측 상단 정렬
		.Offset(FMargin(290, 65, 505, 95)) // ProgressBar 위치와 동일
		[
			SAssignNew(strokeImage, SImage)
			.Image(&strokeBrush)
		]
	];
	
}

void SHealthBarWidget::UpdateHealth (float currHP, float targetHP)
{
	if (targetHP >= 0 && maxHP.Get() > 0)
	{
		progressTargetHP = targetHP;
		
		currPercent = progressTargetHP / maxHP.Get();
		// UE_LOG(LogTemp, Warning, TEXT("퍼센트 값: %f"), currPercent);
		
		healthProgressBar->SetFillColorAndOpacity(GetFillColor(currPercent));
		healthProgressBar->SetPercent(currPercent);

		StartShakeAnimation();
		
		Invalidate(EInvalidateWidgetReason::Paint);
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
    
	if (shakeAnimation.IsPlaying())
	{
		const float animationVal = shakeCurve.GetLerp();
		const float shakeX = 5.0f * FMath::Sin(animationVal * 6.0f * PI);
		const float shakeY = 3.0f * FMath::Cos(animationVal * 6.0f * PI);
		const float dampening = FMath::InterpEaseOut(1.0f, 0.0f, animationVal, 2.0f);
		FVector2D shakeOffset = FVector2D(shakeX, shakeY) * dampening;

		// Stroke에 적용할 더 강한 진폭
		const float strokeShakeX = 10.0f * FMath::Sin(animationVal * 6.0f * PI); // 진폭 증가
		const float strokeShakeY = 8.0f * FMath::Cos(animationVal * 6.0f * PI); // 진폭 증가
		const float strokeDampening = FMath::InterpEaseOut(1.0f, 0.0f, animationVal, 2.0f);
		FVector2D strokeShakeOffset = FVector2D(strokeShakeX, strokeShakeY) * strokeDampening;

		if(currPercent <= 0.35f && strokeImage.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("stroke image 흔들림"));
			strokeImage->SetRenderTransform(FSlateRenderTransform(strokeShakeOffset));
		}
		
		// 오프셋만 적용한 새로운 transform 생성
		if(healthProgressBar.IsValid())
		{
			healthProgressBar->SetRenderTransform(
				FSlateRenderTransform(shakeOffset)
			);
		}
	}
	else
	{
		// 애니메이션이 재생되지 않을 때는 원래 위치로
		if(healthProgressBar.IsValid())
		{
			healthProgressBar->SetRenderTransform(
				FSlateRenderTransform()
			);
		}

		if(strokeImage.IsValid())
		{
			strokeImage->SetRenderTransform(FSlateRenderTransform());
		}
	}
    
	return LayerId;
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
	const FString progressBGTextPath = TEXT("/Game/Assets/HUD/HP/HUD_HP_BarFill_alpha");
	UTexture2D* pgBGTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *progressBGTextPath));
	if(pgBGTex)
	{
		fillBrush = FSlateImageBrush(pgBGTex, FVector2D(403.f, 88.f));
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

	const FString strokeTextPath = TEXT("/Game/Assets/HUD/HP/HUD_HP_BarStroke_resize_alpha");
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

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
