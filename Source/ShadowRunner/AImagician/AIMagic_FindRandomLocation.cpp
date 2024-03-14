// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowRunner/AImagician/AIMagic_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AIMagicController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AIMagic_blackBoard_Keys.h"

UAIMagic_FindRandomLocation::UAIMagic_FindRandomLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UAIMagic_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& Owner_Comp, uint8* Node_Memory)
{
	//get AI controller and its NPC
	auto const cont = Cast<AAIMagicController>(Owner_Comp.GetAIOwner());
	auto const npc = cont->GetPawn();

	//obtain npc location to use as an origin location
	FVector const origin = npc->GetActorLocation();
	FNavLocation loc;

	//get the navigaton system and generate a random location on the NavMesh
	UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
	if(nav_sys->GetRandomPointInNavigableRadius(origin, search_radius, loc, nullptr))
	{
		cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::target_location, loc.Location);
	}

	//finish with success
	FinishLatentTask(Owner_Comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
