// Fill out your copyright notice in the Description page of Project Settings.

#include "StopInvestigating.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoardKeys.h"

UStopInvestigating::UStopInvestigating(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Stop Investigating");
}

EBTNodeResult::Type UStopInvestigating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get targetlocation from blackboard via aI controller
	AEnemyAIController* const controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	controller->GetBlackBoard()->GetValueAsBool(bbKeys::IsInvestigating);

	//finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;

}
