// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "EnemyAIController.h"
#include "GameFramework/Character.h"
#include "BlackBoardKeys.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UFindPlayerLocation::UFindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get player character and the enemy's controller
	ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	//auto const npc = Controller->GetPawn();
	auto const Origin = Controller->GetPawn()->GetActorLocation();

	//getplayer location to use as an origin
	FVector PlayerLocation = Player->GetActorLocation();

	if(SearchRandom)
	{
		FNavLocation location;

		//get the navigation system and generate a random location near the player
		UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if(NavSystem->GetRandomReachablePointInRadius(PlayerLocation, SearchRadius, location, nullptr))
		{
			Controller->GetBlackBoard()->SetValueAsVector(bbKeys::TargetLocation, location.Location);
		}
	}
	else
	{
		if(Controller->GetBlackBoard()->GetValueAsBool(bbKeys::IsMeleeInCheckPointRange))
		{
			UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
			FNavLocation Location;
			if (NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location, nullptr)) {
				Controller->GetBlackBoard()->SetValueAsVector(bbKeys::TargetLocation, Location.Location);

				UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Controller->GetBlackBoard()->GetValueAsVector(bbKeys::TargetLocation));
				
			}
			

		}
		PlayerLocation = Player->GetActorLocation();
		Controller->GetBlackBoard()->SetValueAsVector(bbKeys::TargetLocation, PlayerLocation);
	}

	//finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
