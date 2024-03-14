// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectPatrolPointBTTN.h"
#include "FlyingEnemyTargetPoint.h"
#include "FlyingEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FlyingEnemyCharacter.h"

EBTNodeResult::Type USelectPatrolPointBTTN::ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* nodeMemory)
{
  AFlyingEnemyAIController* AIController = Cast<AFlyingEnemyAIController>(ownerComponent.GetAIOwner());
  AFlyingEnemyCharacter* flyingEnemy = Cast<AFlyingEnemyCharacter>(AIController->GetPawn());
  

  if (AIController)
  {
    UBlackboardComponent* blackboardComponent = AIController->GetBlackboardComponent();
    TArray<AActor*> availableTargetPoints = AIController->GetTargetPoints();
    AFlyingEnemyTargetPoint* currentTargetPoint = nullptr;

    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("AIController->currentTargetPoint = %d"), AIController->currentTargetPoint));

    // Determine the next target point.
    for (int i = 0; i < availableTargetPoints.Num(); ++i) // Iterate through all the target points.
    {
      // Set current target point.
      if (AIController->currentTargetPoint == availableTargetPoints.Num() - 1) // Last target point, rollback to the first one.
      {
        AIController->currentTargetPoint = 0;
      }
      else
      {
        AIController->currentTargetPoint++;
      }
      currentTargetPoint = Cast<AFlyingEnemyTargetPoint>(availableTargetPoints[AIController->currentTargetPoint]);

      // Check if target point is blocked.
      FVector start = flyingEnemy->GetActorLocation();
      FVector end = currentTargetPoint->GetActorLocation();
      FHitResult hitResult;
      FCollisionQueryParams collisionParams;

      bool IsBlocked = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, collisionParams);
      if (IsBlocked)
      {
        currentTargetPoint = nullptr;
      }
      else
      {
        // Target found, break from loop.
        break;
      }
    }

    blackboardComponent->SetValueAsObject("LocationToGo", currentTargetPoint);

    // Get the possessed character.
    //AFlyingEnemyCharacter* flyingEnemy = Cast<AFlyingEnemyCharacter>(AIController->GetPawn());
    //flyingEnemy->SpawnProjectile();

    return EBTNodeResult::Succeeded;
  }

  return EBTNodeResult::Failed;
}