// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowRunner/AImagician/AIMagic_IsShadowInMagicRange.h"
#include "AIMagicController.h"
#include "AIMagic.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIMagic_blackBoard_Keys.h"
#include "ShadowRunner/AIShadowCloneController.h"
#include "ShadowRunner/ShadowCloneCharacter.h"

UAIMagic_IsShadowInMagicRange::UAIMagic_IsShadowInMagicRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Shadow In Magic Range");
}

void UAIMagic_IsShadowInMagicRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	//get NPC
	AAIMagicController* const cont = Cast<AAIMagicController>(OwnerComp.GetAIOwner());
	AAIMagic* const npc = Cast<AAIMagic>(cont->GetPawn());

	//get Player character
	TArray<AActor*> targetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShadowCloneCharacter::StaticClass(), targetPoints);

	//write true or false depending on whether the player is within magic range
	if (targetPoints.IsValidIndex(0) == false)
	{
		cont->get_blackBoard()->SetValueAsBool(magic_bb_keys::shadow_is_in_magic_range, false);
		cont->get_blackBoard()->SetValueAsBool(magic_bb_keys::can_see_shadow, false);
	}
	else if (targetPoints.IsValidIndex(0))
	{
		cont->get_blackBoard()->SetValueAsBool(magic_bb_keys::shadow_is_in_magic_range, npc->GetDistanceTo(targetPoints[0]) <= magic_range);
	}

}