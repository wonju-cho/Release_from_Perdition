// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingEnemyAIController.h"
#include "FlyingEnemyCharacter.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "FlyingEnemyTargetPoint.h"
#include "Misc/OutputDeviceDebug.h"
#include "GameFramework/GameStateBase.h"
#include "ShadowRunnerCharacter.h"

AFlyingEnemyAIController::AFlyingEnemyAIController()
{
  behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("behaviorTreeComponent"));
  blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("blackboardComponent"));

  // Initialize blackboard keys.
  locationToGoKey = "LocationToGo";
  playerLocationKey = "PlayerLocation";
  moveWaitTime = 0.0f;

  currentTargetPoint = 0;

  playerDetected = false;
}

void AFlyingEnemyAIController::OnPossess(APawn* InPawn)
{
  Super::OnPossess(InPawn);

  // For audio system.
  controlledPawn = InPawn;

  //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("In AAIShadowCloneController::OnPossess()")));

  AFlyingEnemyCharacter* AICharacter = Cast<AFlyingEnemyCharacter>(InPawn);
  if (AICharacter)
  {
    if (AICharacter->behaviorTree->BlackboardAsset)
    {
      blackboardComponent->InitializeBlackboard(*(AICharacter->behaviorTree->BlackboardAsset));
    }

    // Populate target point array.
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFlyingEnemyTargetPoint::StaticClass(), targetPoints);

    behaviorTreeComponent->StartTree(*AICharacter->behaviorTree);
  }
}

void AFlyingEnemyAIController::SetPlayerDetected(APawn* pawn)
{
  if (blackboardComponent)
  {
    // For audio system.
    AShadowRunnerCharacter* playerDetect = Cast<AShadowRunnerCharacter>(blackboardComponent->GetValueAsObject(playerLocationKey));
    AGameStateBase* gameState = GetWorld()->GetGameState();
    FOutputDeviceDebug debug;
    int enemyCount = 0;
    if (pawn)
    {
      if(playerDetected == false)
      {
        enemyCount = 1;
        playerDetected = true;
      }
    }
    else
    {
      if(playerDetected == true)
      {
        enemyCount = -1;
        playerDetected = false;
      }
    }
    const FString command = FString::Printf(TEXT("Update_Enemy_Count %d"), enemyCount);
    gameState->CallFunctionByNameWithArguments(*command, debug, this, true);
    // -------------------------------------------
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CloneCharacter detected something")));
    blackboardComponent->SetValueAsObject(playerLocationKey, pawn);
  }
}

void AFlyingEnemyAIController::OnEnemyDeath()
{
  //AShadowRunnerCharacter* playerDetect = Cast<AShadowRunnerCharacter>(blackboardComponent->GetValueAsObject(playerLocationKey));
  if (playerDetected)
  {
    AGameStateBase* gameState = GetWorld()->GetGameState();
    const FString command = FString::Printf(TEXT("Update_Enemy_Count %d"), -1);
    FOutputDeviceDebug debug;
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("1414")));
    gameState->CallFunctionByNameWithArguments(*command, debug, this, true);
  }
}