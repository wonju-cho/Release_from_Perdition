// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIShadowCloneController.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API AAIShadowCloneController : public AAIController
{
	GENERATED_BODY()

	class UBehaviorTreeComponent* behaviorTreeComponent;
	class UBlackboardComponent* blackboardComponent;

	// Blackboard keys.
	UPROPERTY(EditDefaultsOnly, Category = AI)
  FName TargetKey;

	TArray<AActor*> targets;

	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

public:
	AAIShadowCloneController();

	void SetEnemyDetected(APawn* pawn);

	// Inline getter function.
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return blackboardComponent; }
	FORCEINLINE TArray<AActor*> GetTargets() const { return targets; }
};
