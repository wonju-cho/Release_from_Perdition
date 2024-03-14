// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocation.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BlackBoardKeys.h"

UFindRandomLocation::UFindRandomLocation(FObjectInitializer const& ObjectInitializer) {
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get AI controller and its npc
	auto const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	auto const npc = Controller->GetPawn();

	//obtain npc location to use as an origin location
	FVector const Origin = npc->GetActorLocation();

	//get the navigation system and generate a random location on the NavMesh
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation Location;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location, nullptr)) {
		Controller->GetBlackBoard()->SetValueAsVector(bbKeys::TargetLocation, Location.Location);
	}

	//finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
