// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowRunner/AImagician/UpdateShadowLocation.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ShadowRunner/AIShadowCloneController.h"
#include "ShadowRunner/ShadowCloneCharacter.h"
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>
#include "AIMagicController.h"
#include <ShadowRunner/AImagician/AIMagic.h>
#include <ShadowRunner/AImagician/AIMagic_blackBoard_Keys.h>

UUpdateShadowLocation::UUpdateShadowLocation()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Update Shadow Location");
}

void UUpdateShadowLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float dt)
{
	FNavLocation loc;
	/*ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);*/
	auto const cont = Cast<AAIMagicController>(OwnerComp.GetAIOwner());
	AAIMagic* magician = Cast< AAIMagic>(cont->GetPawn());

	TArray<AActor*> targetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShadowCloneCharacter::StaticClass(), targetPoints);
/*FVector  player_location = player->GetActorLocation();*/
	
	//float search_radius = 750.0f;

	//UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());

	//if (/*cont->get_blackBoard()->GetValueAsBool(magic_bb_keys::can_see_shadow) == true &&*/ targetPoints.IsValidIndex(0))
	//{
		FVector Shadow_location = targetPoints[0]->GetActorLocation();
	//	if (nav_sys->GetRandomPointInNavigableRadius(Shadow_location, search_radius, loc, nullptr))
	//	{
			FVector magician_location = magician->GetActorLocation();

			FVector VNew = Shadow_location - magician_location;
			VNew.Normalize();
			float distance = (targetPoints[0]->GetDistanceTo(magician));
			if (distance > 600.f)
			{
				distance = distance - 600.f;
				VNew *= distance;

				VNew = magician_location + VNew;
			}
			else
			{
				VNew = magician_location;
			}

			cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::shadow_target_location, VNew);

	//		float x = magician_location.X - Shadow_location.X;
	//		float y = magician_location.Y - Shadow_location.Y;

	//		if (sqrt(x * x + y * y) == 50.f)
	//		{
	//			FVector magician_location_target = Shadow_location;
	//			cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::shadow_target_location, magician_location_target);
	//		}

			cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::shadow_location, Shadow_location);
	//	}
	//}
	


	//auto const cont = Cast<AAIMagicController>(OwnerComp.GetAIOwner());
	//AAIMagic* magician = Cast< AAIMagic>(cont->GetPawn());

	//TArray<AActor*> targetPoints;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShadowCloneCharacter::StaticClass(), targetPoints);

	//if (cont->get_blackBoard()->GetValueAsBool(magic_bb_keys::can_see_shadow) == true && targetPoints.IsValidIndex(0))
	//{
	//	FVector Shadow_location = targetPoints[0]->GetActorLocation();
	//	float alpha_x = 250.f;
	//	float alpha_y = 250.f;

	//	FVector magician_location_target = Shadow_location;
	//	FVector magician_location = magician->GetActorLocation();

	//	if (magician_location.X == Shadow_location.X && magician_location.Y > Shadow_location.Y)//N
	//	{
	//		magician_location_target.Y += alpha_y;
	//	}
	//	else if (magician_location.X > Shadow_location.X && magician_location.Y > Shadow_location.Y)//NE
	//	{
	//		magician_location_target.X += alpha_x;
	//		magician_location_target.Y += alpha_y;
	//	}
	//	else if (magician_location.X > Shadow_location.X && magician_location.Y == Shadow_location.Y)//E
	//	{
	//		magician_location_target.X += alpha_x;
	//	}
	//	else if (magician_location.X > Shadow_location.X && magician_location.Y < Shadow_location.Y)//SE
	//	{
	//		magician_location_target.X += alpha_x;
	//		magician_location_target.Y -= alpha_y;
	//	}
	//	else if (magician_location.X == Shadow_location.X && magician_location.Y < Shadow_location.Y) //S
	//	{
	//		magician_location_target.Y -= alpha_y;
	//	}
	//	else if (magician_location.X < Shadow_location.X && magician_location.Y < Shadow_location.Y)//SW
	//	{
	//		magician_location_target.X -= alpha_x;
	//		magician_location_target.Y -= alpha_y;
	//	}
	//	else if (magician_location.X < Shadow_location.X && magician_location.Y == Shadow_location.Y)//W
	//	{
	//		magician_location_target.X -= alpha_x;
	//	}
	//	else if (magician_location.X < Shadow_location.X && magician_location.Y > Shadow_location.Y) //NW
	//	{
	//		magician_location_target.X -= alpha_x;
	//		magician_location_target.Y += alpha_y;
	//	}

	//	Shadow_location = targetPoints[0]->GetActorLocation();
	//	cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::shadow_location, Shadow_location);

	//	cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::shadow_target_location, magician_location_target);
	//}
}