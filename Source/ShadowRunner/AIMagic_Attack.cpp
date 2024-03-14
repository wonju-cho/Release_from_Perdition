// Fill out your copyright notice in the Description page of Project Settings.


//#include "AIMagic_Attack.h"
//#include "Kismet/KismetSystemLibrary.h"
//#include "Runtime/Engine/Classes/Engine/World.h"
//#include "Engine/LatentActionManager.h"
//#include "Components/SkeletalMeshComponent.h"
//#include "ShadowRunner/ShadowRunnerCharacter.h"
//#include "Animation/AnimMontage.h"
//#include "Animation/AnimInstance.h"
//#include "AImagician/AIMagic.h"

//advise version -> not the solution
/*
UAIMagic_Attack::UAIMagic_Attack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Magical Attack");
}

EBTNodeResult::Type UAIMagic_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get the NPC
	bNotifyTick = true;
	cont = Cast<AAIMagicController>(OwnerComp.GetAIOwner());

	if (cont)
	{
		blackboardComponent = cont->GetBlackboardComponent();
		magician = Cast<AAIMagic>(cont->GetPawn());
		magician->MagicAttackPlay();

		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
	//if (npc)
	//{
	//	if (montage_has_finished(npc))
	//	{
	//		npc->MagicAttackPlay();
	//	}
	//}
	//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//return EBTNodeResult::Succeeded;
}

void UAIMagic_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (cont)
	{
		if (!montage_has_finished(magician))
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

bool UAIMagic_Attack::montage_has_finished(AAIMagic* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetMontage());
}
*/


// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMagic_Attack.h"
#include "AImagician/AIMagicController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "ShadowRunner/ShadowRunnerCharacter.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "AImagician/AIMagic.h"

UAIMagic_Attack::UAIMagic_Attack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Magical Attack");
}

EBTNodeResult::Type UAIMagic_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get the NPC
	AAIMagicController* const cont = Cast<AAIMagicController>(OwnerComp.GetAIOwner());
	AAIMagic* const npc = Cast<AAIMagic>(cont->GetPawn());

	if (npc)
	{
		if (montage_has_finished(npc))
		{
			npc->MagicAttackPlay();
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UAIMagic_Attack::montage_has_finished(AAIMagic* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetMontage());
}
