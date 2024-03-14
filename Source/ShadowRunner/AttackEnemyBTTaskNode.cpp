// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackEnemyBTTaskNode.h"
#include "AIShadowCloneController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShadowRunnerProjectile.h"
#include "ShadowCloneCharacter.h"

EBTNodeResult::Type UAttackEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* nodeMemory)
{
  AAIShadowCloneController* AIController = Cast<AAIShadowCloneController>(ownerComponent.GetAIOwner());

  if (AIController)
  {
    UBlackboardComponent* blackboardComponent = AIController->GetBlackboardComponent();

    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("In UAttackEnemyBTTaskNode::ExecuteTask()")));

    // Get the possessed character.
    AShadowCloneCharacter* shadow = Cast<AShadowCloneCharacter>(AIController->GetPawn());
    shadow->SpawnProjectile();

    return EBTNodeResult::Succeeded;
  }

  return EBTNodeResult::Failed;
}