// Fill out your copyright notice in the Description page of Project Settings.

#include "ChangeSpeed.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BlackBoardKeys.h"
//#include "Runtime/UMG/Public/UMG.h"

UChangeSpeed::UChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* const  Enemy = Cast<AEnemyCharacter>(Controller->GetPawn());

	if(Controller->GetBlackBoard()->GetValueAsBool(bbKeys::CanSeePlayer))
	{
		Enemy->GetCharacterMovement()->MaxWalkSpeed = Speed;
		Enemy->GetCharacterMovement()->bRequestedMoveUseAcceleration = true;
	}
	else
	{
		Enemy->GetCharacterMovement()->MaxWalkSpeed = 375;
	}

}

FString UChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change the Enemy's Speed");
}
