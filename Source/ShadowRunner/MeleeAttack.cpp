// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttack.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"


UMeleeAttack::UMeleeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const Controller = OwnerComp.GetAIOwner();
	AEnemyCharacter* const Enemy = Cast<AEnemyCharacter>(Controller->GetPawn());

	if(Enemy)
	{
		if(MontageHasFinished(Enemy))
		{
			Enemy->MeleeAttack();
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UMeleeAttack::MontageHasFinished(AEnemyCharacter* const Enemy)
{
	return Enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Enemy->GetMontage());
}
