// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "ShadowrunnerController.generated.h"

/**
 * 
 */

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UAmmoComponent;
class UHealthComponent;

UCLASS()

class AShadowrunnerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShadowrunnerController();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite)
	//AShadowRunnerCharacter* shadow;
	ACharacter* shadow;

	////////////////////////////////////////////////////
	virtual void Tick(float dt) override;
	virtual void SetupInputComponent(class UInputComponent* PlayerInputComponent);
	////////////////////////////////
	void OnFire();

	void OnResetVR();

	void MoveForward(float Val);

	void MoveRight(float Val);

	void SprintStart();
	void SprintEnd();

	bool isSprint = false;
};
