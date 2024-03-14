// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToNearLocation.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnemyCharacter.h"
#include "BlackBoardKeys.h"


UMoveToNearLocation::UMoveToNearLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Move To Near Location");

	MinRadius = 10.f;
	MaxRadius = 15.f;

}

EBTNodeResult::Type UMoveToNearLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* const Enemy = Cast<AEnemyCharacter>(Controller->GetPawn());

	//FVector NearLocation = Enemy->GetActorLocation();

	//NearLocation.X += (rand() / (RAND_MAX/(MaxRadius - MinRadius)));
	//NearLocation.Y += (rand() / (RAND_MAX/(MaxRadius - MinRadius)));
	FVector NearLocation = FVector();

	Controller->GetBlackBoard()->SetValueAsVector(bbKeys::NearLocation, NearLocation);

	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, NearLocation);

	Controller->GetBlackBoard()->SetValueAsBool(bbKeys::IsEnemyInRange, false);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
