// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyToTargetPointBTTN.h"
#include "FlyingEnemyTargetPoint.h"
#include "FlyingEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FlyingEnemyCharacter.h"
#include "ShadowRunnerCharacter.h"

EBTNodeResult::Type UFlyToTargetPointBTTN::ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* nodeMemory)
{
  // Set this so that Tick() would be called.
  bNotifyTick = true;

  AIController = Cast<AFlyingEnemyAIController>(ownerComponent.GetAIOwner());

  if (AIController)
  {
    blackboardComponent = AIController->GetBlackboardComponent();
    currentPoint = Cast<AFlyingEnemyTargetPoint>(blackboardComponent->GetValueAsObject("LocationToGo"));
    enemy = Cast<AFlyingEnemyCharacter>(AIController->GetPawn());

    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Check 1")));

    // Set animation.
    enemy->PlayMoveAnimation();

    return EBTNodeResult::InProgress;
  }

  return EBTNodeResult::Failed;
}

void UFlyToTargetPointBTTN::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

  if (AIController)
  {
    // Move to target point.
    if (enemy && currentPoint)
    {
      // Set direction.
      direction = FVector(currentPoint->GetActorLocation() - enemy->GetActorLocation());
      direction.Normalize();

      // Add sine wave movement.
      float time = GetWorld()->GetTimeSeconds() * enemy->GetWaveTurnRate(); // Double the turn rate.
      
      direction += FVector(0.0f, 0.0f, FMath::Cos(time));
      direction.Normalize();

      // Set velocity.
      UMovementComponent* MovementComponent = Cast<UMovementComponent>(enemy->GetComponentByClass(UMovementComponent::StaticClass()));
      if (MovementComponent)
      {
        MovementComponent->Velocity = direction * enemy->GetFlySpeed();
      }

      // Check if the enemy reached the target point.
      if (FMath::IsNearlyEqual(enemy->GetActorLocation().X, currentPoint->GetActorLocation().X, 50.0f)
        && FMath::IsNearlyEqual(enemy->GetActorLocation().Y, currentPoint->GetActorLocation().Y, 50.0f)
        && FMath::IsNearlyEqual(enemy->GetActorLocation().Z, currentPoint->GetActorLocation().Z, 50.0f))
      {
        if (MovementComponent)
        {
          MovementComponent->Velocity = FVector(0.0f);
          FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Finished")));
      }

      // Check if the player is in range.
      AShadowRunnerCharacter* player = Cast<AShadowRunnerCharacter>(blackboardComponent->GetValueAsObject("PlayerLocation"));
      if (player)
      {
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Detected")));
        if (MovementComponent)
        {
          MovementComponent->Velocity = FVector(0.0f);
          enemy->ResetAnimFlags();
          FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
      }
    }

    
  }
}