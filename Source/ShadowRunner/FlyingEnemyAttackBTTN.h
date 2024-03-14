// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FlyingEnemyAttackBTTN.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UFlyingEnemyAttackBTTN : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* nodeMemory) override;
};
