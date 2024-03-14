// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyCharacter.h"
#include "MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UMeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UMeleeAttack(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool MontageHasFinished(AEnemyCharacter* const Enemy);
	
};
