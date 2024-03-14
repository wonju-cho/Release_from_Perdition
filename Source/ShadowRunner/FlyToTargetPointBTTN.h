// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FlyToTargetPointBTTN.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UFlyToTargetPointBTTN : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* nodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class AFlyingEnemyAIController* AIController;
	class UBlackboardComponent* blackboardComponent;
	class AFlyingEnemyTargetPoint* currentPoint;
	class AFlyingEnemyCharacter* enemy;
	FVector direction;
	float speed = 300.0f;
	float waveTurnRate = 3.0f;
};
