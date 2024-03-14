// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FlyingEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API AFlyingEnemyAIController : public AAIController
{
	GENERATED_BODY()

	class UBehaviorTreeComponent* behaviorTreeComponent;
	class UBlackboardComponent* blackboardComponent;

	// Blackboard keys.
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName locationToGoKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName playerLocationKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	float moveWaitTime;

	// Other variables.
	TArray<AActor*> targetPoints;

	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

	// For audio system.
	APawn* controlledPawn;

public:
	AFlyingEnemyAIController();

	void SetPlayerDetected(APawn* pawn);

	int32 currentTargetPoint;

	// Inline getter function.
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return blackboardComponent; }
	FORCEINLINE TArray<AActor*> GetTargetPoints() const { return targetPoints; }

	// Adam's changes
	bool playerDetected;
	//----------------------
	// For audio system.
	void OnEnemyDeath();
};
