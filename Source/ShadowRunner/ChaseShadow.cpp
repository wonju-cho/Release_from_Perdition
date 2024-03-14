// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseShadow.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BlackBoardKeys.h"
#include "ShadowCloneCharacter.h"
#include "Kismet/GameplayStatics.h"


UChaseShadow::UChaseShadow(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Shadow");
}

EBTNodeResult::Type UChaseShadow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get TargetLocation from blackboard via AI controller
	AEnemyAIController* const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	//move to the player's location
	TArray<AActor*> targetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShadowCloneCharacter::StaticClass(), targetPoints);

	if(targetPoints.IsValidIndex(0))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Controller->GetBlackBoard()->GetValueAsVector(bbKeys::ShadowLocation));
	}
	else
	{
		Controller->GetBlackBoard()->SetValueAsBool(bbKeys::CanSeeShadow, false);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
