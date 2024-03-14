// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ChaseShadow.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UChaseShadow : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UChaseShadow(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
