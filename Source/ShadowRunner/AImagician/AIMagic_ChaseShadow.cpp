// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowRunner/AImagician/AIMagic_ChaseShadow.h"
#include "AIMagicController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIMagic_blackBoard_Keys.h"
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>
#include <ShadowRunner/AImagician/AIMagic.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ShadowRunner/AIShadowCloneController.h"
#include "ShadowRunner/ShadowCloneCharacter.h"

UAIMagic_ChaseShadow::UAIMagic_ChaseShadow(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Magician Chase Shadow");
}

EBTNodeResult::Type UAIMagic_ChaseShadow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	//get TargetLocation from blackboard via AI controller
	AAIMagicController* const cont = Cast<AAIMagicController>(OwnerComp.GetOwner());
	AAIMagic* magician = Cast< AAIMagic>(cont->GetPawn());

	TArray<AActor*> targetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShadowCloneCharacter::StaticClass(), targetPoints);

	if (targetPoints.IsValidIndex(0))
	{
		FVector Shadow_location = targetPoints[0]->GetActorLocation();

		FVector magician_location = magician->GetActorLocation();

		float x = magician_location.X - Shadow_location.X;
		float y = magician_location.Y - Shadow_location.Y;

		FVector magician_location_target = Shadow_location;
		cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::shadow_target_location, magician_location_target);

		cont->get_blackBoard()->SetValueAsVector(magic_bb_keys::shadow_location, Shadow_location);


		FVector const enemy_to_go_location = cont->get_blackBoard()->GetValueAsVector(magic_bb_keys::shadow_target_location);

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, enemy_to_go_location);

		//move to the player's location

		//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		cont->get_blackBoard()->SetValueAsBool(magic_bb_keys::can_see_shadow, false);
		cont->get_blackBoard()->SetValueAsBool(magic_bb_keys::shadow_is_in_magic_range, false);
		//FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	return EBTNodeResult::Succeeded;

}
