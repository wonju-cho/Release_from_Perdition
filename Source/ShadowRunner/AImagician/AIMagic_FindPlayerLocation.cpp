// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowRunner/AImagician/AIMagic_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AIMagicController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AIMagic_blackBoard_Keys.h"
#include <ShadowRunner/AImagician/AIMagic.h>


UAIMagic_FindPlayerLocation::UAIMagic_FindPlayerLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UAIMagic_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	//get player character and the AIMagic's controller
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const cont = Cast<AAIMagicController>(owner_comp.GetAIOwner());
	//get player location to use as an origin
	FVector  player_location = player->GetActorLocation();
	player_location = player->GetActorLocation();
	cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::player_location, player_location);

	if(search_random)
	{
		FNavLocation loc;
		//get the navigation system and generate a random location near the player
		UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
		if(nav_sys->GetRandomPointInNavigableRadius(player_location, search_radius, loc, nullptr))
		{
			cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::target_location, loc.Location);
		}
	}
	else
	{
		//Not Using this cpp now, but not sure 
		//float alpha_x = 450.f;
		//float alpha_y = 450.f;
		AAIMagic* magician = Cast< AAIMagic>(cont->GetPawn());
		//FVector magician_location_target = player_location;
		FVector magician_location = magician->GetActorLocation();
		float x = magician_location.X - player_location.X;
		float y = magician_location.Y - player_location.Y;

		if (sqrt(x * x + y * y) == 450.f)
		{
			FVector magician_location_target = player_location;
			cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::target_location, magician_location_target);
		}
		
		//if (magician_location.X == player_location.X && magician_location.Y > player_location.Y)//N
		//{
		//	magician_location_target.Y += alpha_y;
		//}
		//else if (magician_location.X > player_location.X && magician_location.Y > player_location.Y)//NE
		//{
		//	magician_location_target.X += alpha_x;
		//	magician_location_target.Y += alpha_y;
		//}
		//else if (magician_location.X > player_location.X && magician_location.Y == player_location.Y)//E
		//{
		//	magician_location_target.X += alpha_x;
		//}
		//else if (magician_location.X > player_location.X && magician_location.Y < player_location.Y )//SE
		//{
		//	magician_location_target.X += alpha_x;
		//	magician_location_target.Y -= alpha_y;
		//}
		//else if (magician_location.X == player_location.X && magician_location.Y < player_location.Y) //S
		//{
		//	magician_location_target.Y -= alpha_y;
		//}
		//else if (magician_location.X < player_location.X && magician_location.Y < player_location.Y)//SW
		//{
		//	magician_location_target.X -= alpha_x;
		//	magician_location_target.Y -= alpha_y;
		//}
		//else if (magician_location.X < player_location.X && magician_location.Y == player_location.Y)//W
		//{
		//	magician_location_target.X -= alpha_x;
		//}
		//else if (magician_location.X < player_location.X && magician_location.Y > player_location.Y) //NW
		//{
		//	magician_location_target.X -= alpha_x;
		//	magician_location_target.Y += alpha_y;
		//}

		player_location = player->GetActorLocation();
		cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::player_location, player_location);

	}
	//finish with success
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}