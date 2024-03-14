// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowRunner/AImagician/IsPlayerInMagicRange.h"
#include "AIMagicController.h"
#include "AIMagic.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIMagic_blackBoard_Keys.h"


UIsPlayerInMagicRange::UIsPlayerInMagicRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Shadow In Magic Range");
}

void UIsPlayerInMagicRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	//get NPC
	AAIMagicController* const cont = Cast<AAIMagicController>(OwnerComp.GetAIOwner());
	AAIMagic* const npc = Cast<AAIMagic>(cont->GetPawn());

	//get Player character
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	//write true or false depending on whether the player is within magic range
	cont->get_blackBoard()->SetValueAsBool(magic_bb_keys::player_is_in_magic_range, npc->GetDistanceTo(player) <= magic_range);
}

