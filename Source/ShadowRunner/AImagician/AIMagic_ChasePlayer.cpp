// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowRunner/AImagician/AIMagic_ChasePlayer.h"
#include "AIMagicController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIMagic_blackBoard_Keys.h"
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>
#include "ShadowRunner/AImagician/AIMagic.h"
#include <Runtime/Core/Public/Math/Vector.h>
UAIMagic_ChasePlayer::UAIMagic_ChasePlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Chase Player");
}
EBTNodeResult::Type UAIMagic_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	//get TargetLocation from blackboard via AI controller
	AAIMagicController* const cont = Cast<AAIMagicController>(OwnerComp.GetOwner());

	/*ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector  player_location = player->GetActorLocation();
	AAIMagic* magician = Cast< AAIMagic>(cont->GetPawn());
	FVector magician_location = magician->GetActorLocation();

	FVector VNew = player_location - magician_location;
	VNew.Normalize();
	float distance = 8000.f;

	VNew *= distance;

	VNew = magician_location + VNew;
	cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::target_location, VNew);*/

	FVector const enemy_to_go_location = cont->get_blackBoard()->GetValueAsVector(magic_bb_keys::target_location);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, enemy_to_go_location);



	//move to the player's location

	//finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
	//FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//return EBTNodeResult::Failed;
}
