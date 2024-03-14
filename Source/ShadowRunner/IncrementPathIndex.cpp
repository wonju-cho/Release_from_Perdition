// Fill out your copyright notice in the Description page of Project Settings.

#include "IncrementPathIndex.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoardKeys.h"
#include <cmath>

UIncrementPathIndex::UIncrementPathIndex(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UIncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get the ai controller
	AEnemyAIController* const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* const Enemy = Cast<AEnemyCharacter>(Controller->GetPawn());

	int const noOfPoints = Enemy->GetPatrolPath()->Num();
	int const minIndex = 0;
	int const maxIndex = noOfPoints - 1;

	//get and set the black board index key
	int index = Controller->GetBlackBoard()->GetValueAsInt(bbKeys::PatrolPathIndex);

	if(BiDirectional)
	{
		if (index >= maxIndex && Direction == EDirectionType::Forward)
		{
			Direction = EDirectionType::Reverse;
		}
		else if (index == minIndex && Direction == EDirectionType::Reverse)
		{
			Direction = EDirectionType::Forward;
		}
	}

	 
	Controller->GetBlackBoard()->SetValueAsInt(bbKeys::PatrolPathIndex, 
		(Direction == EDirectionType::Forward ? std::abs(++index) : std::abs(--index)) % noOfPoints);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
