// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChase.h"
#include "ShadowRunnerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnemyAIController.h"
#include "BlackBoardKeys.h"

UPlayerChase::UPlayerChase(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UPlayerChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get targetlocation from blackboard via aI controller
	AEnemyAIController* const controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	//FVector const PlayerLocation = controller->GetBlackBoard()->GetValueAsVector(bbKeys::TargetLocation);

	//move to the player's location
	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, PlayerLocation);

	//finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;

}
