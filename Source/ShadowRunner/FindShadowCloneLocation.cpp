// Fill out your copyright notice in the Description page of Project Settings.


#include "FindShadowCloneLocation.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "ShadowCloneCharacter.h"
#include "BlackBoardKeys.h"


UFindShadowCloneLocation::UFindShadowCloneLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Shadow Clone Location");
}

EBTNodeResult::Type UFindShadowCloneLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	TArray<AActor*> targetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShadowCloneCharacter::StaticClass(), targetPoints);


	//if(bbKeys::CanSeePlayer && targetPoints.IsValidIndex(0))
	if(Controller->GetBlackBoard()->GetValueAsBool(bbKeys::CanSeeShadow) && targetPoints.IsValidIndex(0))
	{
		Controller->GetBlackBoard()->SetValueAsVector(bbKeys::ShadowLocation, targetPoints[0]->GetActorLocation());
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;

}
