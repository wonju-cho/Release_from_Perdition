// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectEnemy_BTTN.h"
#include "AIShadowCloneController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UDetectEnemy_BTTN::ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* nodeMemory)
{
  AAIShadowCloneController* AIController = Cast<AAIShadowCloneController>(ownerComponent.GetAIOwner());

  if (AIController)
  {
    UBlackboardComponent* blackboardComponent = AIController->GetBlackboardComponent();

    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("In UDetectEnemy_BTTN::ExecuteTask()")));

    return EBTNodeResult::Succeeded;
  }

  return EBTNodeResult::Failed;
}
