// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingEnemyAttackBTTN.h"
#include "FlyingEnemyTargetPoint.h"
#include "FlyingEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FlyingEnemyCharacter.h"
#include "ShadowRunnerCharacter.h"

EBTNodeResult::Type UFlyingEnemyAttackBTTN::ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* nodeMemory)
{
  AFlyingEnemyAIController* AIController = Cast<AFlyingEnemyAIController>(ownerComponent.GetAIOwner());

  if (AIController)
  {
    UBlackboardComponent* blackboardComponent = AIController->GetBlackboardComponent();
    AShadowRunnerCharacter* player = Cast<AShadowRunnerCharacter>(blackboardComponent->GetValueAsObject("PlayerLocation"));
    AFlyingEnemyCharacter* enemy = Cast<AFlyingEnemyCharacter>(AIController->GetPawn());

    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Check 1")));

    if (enemy && player)
    {
      // Set direction.
      FVector direction = FVector(player->GetActorLocation() - enemy->GetActorLocation());
      direction.Normalize();
      enemy->SetActorRotation(direction.Rotation());

      // Set velocity.
      UMovementComponent* MovementComponent = Cast<UMovementComponent>(enemy->GetComponentByClass(UMovementComponent::StaticClass()));
      if (MovementComponent)
      {
        MovementComponent->Velocity = FVector(0.0f);
      }

      enemy->SpawnProjectile();
    }

    return EBTNodeResult::Succeeded;
  }

  return EBTNodeResult::Failed;
}