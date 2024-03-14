// Fill out your copyright notice in the Description page of Project Settings.


#include "IsShadowInMeleeRange.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "ShadowCloneCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShadowRunnerCharacter.h"
#include "AIShadowCloneController.h"
#include "BlackBoardKeys.h"

UIsShadowInMeleeRange::UIsShadowInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Shadow In Melee Range");
}

void UIsShadowInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	//get Enemy
	AEnemyAIController* const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* const Enemy = Cast<AEnemyCharacter>(Controller->GetPawn());

	TArray<AActor*> targetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShadowCloneCharacter::StaticClass(), targetPoints);

	if(targetPoints.IsValidIndex(0))
	{
		Controller->GetBlackBoard()->SetValueAsBool(bbKeys::ShadowIsInMeleeRange, Enemy->GetDistanceTo(targetPoints[0]) <= MeleeRange);
	}
	else if(targetPoints.IsValidIndex(0) == false)
	{
		Controller->GetBlackBoard()->SetValueAsBool(bbKeys::ShadowIsInMeleeRange, false);
	}


}
