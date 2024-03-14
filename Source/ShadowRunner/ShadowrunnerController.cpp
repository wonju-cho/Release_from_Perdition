// Fill out your copyright notice in the Description page of Project Settings.

#include "ShadowrunnerController.h"
#include "ShadowRunnerCharacter.h"
#include "ShadowRunnerProjectile.h"
#include "Animation/AnimInstance.h"
#include "ShadowRunnerGameMode.h"

//#include "ShadowrunnerController.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/InputSettings.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
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

	bShowMouseCursor = true;
}

void AShadowrunnerController::Tick(float dt)
{
	Super::Tick(dt);
}

void AShadowrunnerController::SetupInputComponent(class UInputComponent* PlayerInputComponent)
{
	//Super::SetupInputComponent();

	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AShadowrunnerController::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShadowrunnerController::MoveRight);
}

void AShadowrunnerController::OnFire()
{

}

void AShadowrunnerController::OnResetVR()
{

}

void AShadowrunnerController::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		if (isSprint)
		{
			Val *= 2;
		}
		// add movement in that direction
		shadow->AddMovementInput(shadow->GetActorForwardVector(), Val / 2);
	}
}

void AShadowrunnerController::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		if (isSprint)
		{
			Val *= 2;
		}
		// add movement in that direction
		shadow->AddMovementInput(shadow->GetActorRightVector(), Val / 2);
	}

}

void AShadowrunnerController::SprintStart()
{
	isSprint = true;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("sprintYes")));
}

void AShadowrunnerController::SprintEnd()
{
	isSprint = false;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("sprintNo")));
}

