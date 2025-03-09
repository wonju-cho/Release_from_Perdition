// Fill out your copyright notice in the Description page of Project Settings.

#include "ShadowrunnerController.h"
#include "ShadowRunnerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

//DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

AShadowrunnerController::AShadowrunnerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AShadowrunnerController::BeginPlay()
{
	Super::BeginPlay();

	//커서 안보이게 설정
	bShowMouseCursor = false;
	
	FInputModeGameAndUI inputMode;
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
}

void AShadowrunnerController::Tick(float dt)
{
	Super::Tick(dt);
}

void AShadowrunnerController::SetupInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupInputComponent();

	check(PlayerInputComponent);
	
}