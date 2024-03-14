// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FlyingEnemyChargeToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UFlyingEnemyChargeToPlayer : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* nodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class AFlyingEnemyAIController* AIController;
	class UBlackboardComponent* blackboardComponent;
	class AShadowRunnerCharacter* player;
	class AFlyingEnemyCharacter* enemy;
	FVector direction;
	FVector destination;
	float chargeSpeed = 3000.0f;
	// Timer handle for changing material.
	struct FTimerHandle chargeTimerHandle;
	float chargeWaitTime;
	void ChargeTimerCallback();
	bool bAbort;
};
