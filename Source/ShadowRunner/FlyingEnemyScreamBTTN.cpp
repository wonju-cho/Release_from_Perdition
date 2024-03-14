// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingEnemyScreamBTTN.h"
#include "FlyingEnemyTargetPoint.h"
#include "FlyingEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FlyingEnemyCharacter.h"
#include "ShadowRunnerCharacter.h"
#include "Kismet/GameplayStatics.h"

extern float volumeControlEnemy;

EBTNodeResult::Type UFlyingEnemyScreamBTTN::ExecuteTask(UBehaviorTreeComponent& ownerComponent, uint8* nodeMemory)
{
  // Set this so that Tick() would be called.
  bNotifyTick = true;

  AIController = Cast<AFlyingEnemyAIController>(ownerComponent.GetAIOwner());

  if (AIController)
  {
    blackboardComponent = AIController->GetBlackboardComponent();
    player = Cast<AShadowRunnerCharacter>(blackboardComponent->GetValueAsObject("PlayerLocation"));
    enemy = Cast<AFlyingEnemyCharacter>(AIController->GetPawn());

    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Check 1")));

    if (enemy && player)
    {
      // Check if target point is blocked.
      FVector start = enemy->GetActorLocation();
      FVector end = player->GetActorLocation();
      FHitResult hitResult;
      FCollisionQueryParams collisionParams;

      bool IsBlocked = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, collisionParams);
      if (!IsBlocked)
      {
        if (enemy->ScreamSound)
        {
          screamTimer = 2.0f;
          UGameplayStatics::PlaySoundAtLocation(enemy, enemy->ScreamSound, enemy->GetActorLocation(), volumeControlEnemy);
          return EBTNodeResult::InProgress;
        }
      }
    }
  }

  return EBTNodeResult::Failed;
}

void UFlyingEnemyScreamBTTN::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

  if (enemy && player)
  {
    // Set direction.
    FVector direction = FVector(player->GetActorLocation() - enemy->GetActorLocation());
    direction.Normalize();
    enemy->SetActorRotation(direction.Rotation());

    // Set animation.
    enemy->PlayScreamAnimation();

    screamTimer -= DeltaSeconds;
    if (screamTimer <= 0.0f)
    {
      enemy->ResetAnimFlags();
      FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
  }
}