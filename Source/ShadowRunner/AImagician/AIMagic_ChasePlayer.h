// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "ShadowRunner/ShadowRunnerCharacter.h"
#include "AIMagic_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SHADOWRUNNER_API UAIMagic_ChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UAIMagic_ChasePlayer(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
