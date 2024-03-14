// Fill out your copyright notice in the Description page of Project Settings.


#include "AIShadowCloneController.h"
#include "ShadowCloneCharacter.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"

AAIShadowCloneController::AAIShadowCloneController()
{
  behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("behaviorTreeComponent"));
  blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("blackboardComponent"));

  // Initialize blackboard keys.
  TargetKey = "Target";
}

void AAIShadowCloneController::OnPossess(APawn* InPawn)
{
  Super::OnPossess(InPawn);

  //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("In AAIShadowCloneController::OnPossess()")));

  AShadowCloneCharacter* AICharacter = Cast<AShadowCloneCharacter>(InPawn);
  if (AICharacter)
  {
    if (AICharacter->behaviorTree->BlackboardAsset)
    {
      blackboardComponent->InitializeBlackboard(*(AICharacter->behaviorTree->BlackboardAsset));
    }

    // Populate enemy array.
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), targets);
    //UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShadowCloneTargetPoint::StaticClass(), targets);

    behaviorTreeComponent->StartTree(*AICharacter->behaviorTree);
  }
}

void AAIShadowCloneController::SetEnemyDetected(APawn* pawn)
{
  if (blackboardComponent)
  {
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CloneCharacter detected something")));
    blackboardComponent->SetValueAsObject(TargetKey, pawn);
  }
}