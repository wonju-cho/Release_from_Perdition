// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "FlyingEnemyIsPlayerInRangeBTSBB.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UFlyingEnemyIsPlayerInRangeBTSBB : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UFlyingEnemyIsPlayerInRangeBTSBB();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
