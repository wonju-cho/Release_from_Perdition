// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingEnemyIsPlayerInRangeBTSBB.h"
#include "FlyingEnemyAIController.h"
#include "FlyingEnemyCharacter.h"

UFlyingEnemyIsPlayerInRangeBTSBB::UFlyingEnemyIsPlayerInRangeBTSBB()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("FlyingEnemy::IsPlayerInRange");
}

void UFlyingEnemyIsPlayerInRangeBTSBB::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
}