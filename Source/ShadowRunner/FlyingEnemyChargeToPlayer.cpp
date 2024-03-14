// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingEnemyChargeToPlayer.h"
#include "FlyingEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FlyingEnemyCharacter.h"
#include "ShadowRunnerCharacter.h"

EBTNodeResult::Type UFlyingEnemyChargeToPlayer::ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* nodeMemory)
{
  // Set this so that Tick() would be called.
  bNotifyTick = true;

  AIController = Cast<AFlyingEnemyAIController>(ownerComponent.GetAIOwner());

  chargeWaitTime = 3.0f;
  bAbort = false;

  if (AIController)
  {
    blackboardComponent = AIController->GetBlackboardComponent();
    player = Cast<AShadowRunnerCharacter>(blackboardComponent->GetValueAsObject("PlayerLocation"));
    enemy = Cast<AFlyingEnemyCharacter>(AIController->GetPawn());

    if (enemy && player)
    {
      destination = player->GetActorLocation();
    }
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Check 1")));

    // Set animation.
    enemy->PlayChargeAnimation();

    return EBTNodeResult::InProgress;
  }

  return EBTNodeResult::Failed;
}

void UFlyingEnemyChargeToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

  if (AIController)
  {
    // Charge to destination.
    if (enemy)
    {
      // Set direction.
      direction = FVector(destination - enemy->GetActorLocation());
      direction.Normalize();

      //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("destination %f %f %f"), destination.X, destination.Y, destination.Z));

      // Set velocity.
      //UMovementComponent* MovementComponent = Cast<UMovementComponent>(enemy->GetComponentByClass(UMovementComponent::StaticClass()));
      UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(enemy->GetCharacterMovement());
      if (MovementComponent)
      {
        MovementComponent->Velocity = direction * enemy->GetChargeSpeed();
      }

      if (!GetWorld()->GetTimerManager().IsTimerActive(chargeTimerHandle))
      {
        GetWorld()->GetTimerManager().SetTimer(chargeTimerHandle, this, &UFlyingEnemyChargeToPlayer::ChargeTimerCallback, chargeWaitTime, false);
      }

      if (bAbort)
      {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
      }

      // Check if the enemy reached the destination.
      if (FMath::IsNearlyEqual(enemy->GetActorLocation().X, destination.X, 50.0f)
        && FMath::IsNearlyEqual(enemy->GetActorLocation().Y, destination.Y, 50.0f)
        && FMath::IsNearlyEqual(enemy->GetActorLocation().Z, destination.Z, 50.0f))
      {
        if (MovementComponent)
        {
          MovementComponent->Velocity = FVector(0.0f);
          enemy->ResetAnimFlags();
          FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Finished")));
      }
    }
  }
}

void UFlyingEnemyChargeToPlayer::ChargeTimerCallback()
{
  bAbort = true;
}