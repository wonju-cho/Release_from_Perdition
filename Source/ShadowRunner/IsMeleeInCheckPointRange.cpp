// Fill out your copyright notice in the Description page of Project Settings.


#include "IsMeleeInCheckPointRange.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "CheckPoint.h"
#include "BlackBoardKeys.h"
#include "BehaviorTree/BlackboardComponent.h"

UIsMeleeInCheckPointRange::UIsMeleeInCheckPointRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Melee In Check Point Range");
}

void UIsMeleeInCheckPointRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AEnemyAIController* const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* const Enemy = Cast<AEnemyCharacter>(Controller->GetPawn());

	TArray<AActor*> targetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckPoint::StaticClass(), targetPoints);
	bool isInRange = false;

	for (int i = 0; i < targetPoints.Num(); i++)
	{
		if (targetPoints.IsValidIndex(i))
		{
			if (Enemy->GetDistanceTo(targetPoints[i]) <= CheckPointRange)
			{
				isInRange = true;
			}
		}
	}

	if (isInRange)
	{
		Controller->GetBlackBoard()->SetValueAsBool(bbKeys::IsMeleeInCheckPointRange, true);
	}
	else
	{
		Controller->GetBlackBoard()->SetValueAsBool(bbKeys::IsMeleeInCheckPointRange, false);
	}
}

