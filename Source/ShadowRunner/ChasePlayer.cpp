// Fill out your copyright notice in the Description page of Project Settings.

#include "ChasePlayer.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BlackBoardKeys.h"

UChasePlayer::UChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get TargetLocation from blackboard via AI controller
	AEnemyAIController* const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	FVector const PlayerLocation = Controller->GetBlackBoard()->GetValueAsVector(bbKeys::TargetLocation);

	//move to the player's location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, PlayerLocation);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
