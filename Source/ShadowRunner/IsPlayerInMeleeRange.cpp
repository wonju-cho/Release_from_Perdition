// Fill out your copyright notice in the Description page of Project Settings.

#include "IsPlayerInMeleeRange.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIShadowCloneController.h"
#include "BlackBoardKeys.h"


UIsPlayerInMeleeRange::UIsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UIsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	//get Enemy
	AEnemyAIController* const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* const Enemy = Cast<AEnemyCharacter>(Controller->GetPawn());

	//get player character
	ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	//write true or false depending on whether the player is within melee range
	Controller->GetBlackBoard()->SetValueAsBool(bbKeys::PlayerIsInMeleeRange, Enemy->GetDistanceTo(Player) <= MeleeRange);

}
