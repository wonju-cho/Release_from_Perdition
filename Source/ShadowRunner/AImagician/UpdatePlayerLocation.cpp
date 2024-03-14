// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowRunner/AImagician/UpdatePlayerLocation.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "AIMagicController.h"
#include <ShadowRunner/AImagician/AIMagic.h>
#include <ShadowRunner/AImagician/AIMagic_blackBoard_Keys.h>
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>

UUpdatePlayerLocation::UUpdatePlayerLocation()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Update Player Location");

}

void UUpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float dt)
{
	/*
	//Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	auto const cont = Cast<AAIMagicController>(OwnerComp.GetAIOwner());
	//ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AShadowRunnerCharacter* const player = Cast< AShadowRunnerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	float alpha_x = 450.f;
	float alpha_y = 450.f;
	FVector  player_location = player->GetActorLocation();
	AAIMagic* magician = Cast< AAIMagic>(cont->GetPawn());
	FVector magician_location_target = player_location;
	FVector magician_location = magician->GetActorLocation();

	if (cont->get_blackBoard()->GetValueAsBool(magic_bb_keys::can_see_shadow))
	{
		if (magician_location.X == player_location.X && magician_location.Y > player_location.Y)//N
		{
			magician_location_target.Y += alpha_y;
		}
		else if (magician_location.X > player_location.X && magician_location.Y > player_location.Y)//NE
		{
			magician_location_target.X += alpha_x;
				magician_location_target.Y += alpha_y;
		}
		else if (magician_location.X > player_location.X && magician_location.Y == player_location.Y)//E
		{
			magician_location_target.X += alpha_x;
		}
		else if (magician_location.X > player_location.X && magician_location.Y < player_location.Y)//SE
		{
			magician_location_target.X += alpha_x;
			magician_location_target.Y -= alpha_y;
		}
		else if (magician_location.X == player_location.X && magician_location.Y < player_location.Y) //S
		{
			magician_location_target.Y -= alpha_y;
		}
		else if (magician_location.X < player_location.X && magician_location.Y < player_location.Y)//SW
		{
			magician_location_target.X -= alpha_x;
			magician_location_target.Y -= alpha_y;
		}
		else if (magician_location.X < player_location.X && magician_location.Y == player_location.Y)//W
		{
			magician_location_target.X -= alpha_x;
		}
		else if (magician_location.X < player_location.X && magician_location.Y > player_location.Y) //NW
		{
			magician_location_target.X -= alpha_x;
			magician_location_target.Y += alpha_y;
		}

		player_location = player->GetActorLocation();
		cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::player_location, player_location);

		cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::target_location, magician_location_target);
	}
	*/
	//FNavLocation loc;
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	auto const cont = Cast<AAIMagicController>(OwnerComp.GetAIOwner());

	//ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector  player_location = player->GetActorLocation();
	AAIMagic* magician = Cast< AAIMagic>(cont->GetPawn());
	FVector magician_location = magician->GetActorLocation();

	FVector VNew = player_location - magician_location;
	VNew.Normalize();
	float distance = (player->GetDistanceTo(magician));
	if (distance > 900.f)
	{
		distance = distance - 900.f;
		VNew *= distance;

		VNew = magician_location + VNew;
		//cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::target_location, VNew);
	}
	else
	{
		VNew = magician_location;
	}
	/*if (cont->get_blackBoard()->GetValueAsBool(magic_bb_keys::got_attack_behind))
	{
		distance = 1.f;
	}*/


	cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::target_location, VNew);


	cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::player_location, player_location);
	//}



}
