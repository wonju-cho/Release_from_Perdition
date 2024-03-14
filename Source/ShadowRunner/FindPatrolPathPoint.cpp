// Fill out your copyright notice in the Description page of Project Settings.

#include "FindPatrolPathPoint.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "BlackBoardKeys.h"

UFindPatrolPathPoint::UFindPatrolPathPoint(FObjectInitializer const& ObejctIntializer)
{
	NodeName = TEXT("Find Patrol Path Point");
}

EBTNodeResult::Type UFindPatrolPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get the AI controller for the patrolling Enemy
	AEnemyAIController* const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	//get the current patrol path index from the blackboard
	int const index = Controller->GetBlackBoard()->GetValueAsInt(bbKeys::PatrolPathIndex);

	//use the index to get the current patrol path from the NPC's reference to the patrol path
	AEnemyCharacter* const Enemy = Cast<AEnemyCharacter>(Controller->GetPawn());
	FVector const Point = Enemy->GetPatrolPath()->GetPatrolPoint(index);

	//transform this point to a global position using its parent
	FVector const GlobalPoint = Enemy->GetPatrolPath()->GetActorTransform().TransformPosition(Point);

	//wrtie the current global path point to the blackboard
	Controller->GetBlackBoard()->SetValueAsVector(bbKeys::PatrolPathVector, GlobalPoint);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}