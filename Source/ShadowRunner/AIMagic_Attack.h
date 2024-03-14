// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once
//
//#include "CoreMinimal.h"
//#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
//#include "AImagician/AIMagicController.h"
//#include "ShadowRunner/ShadowRunnerCharacter.h"
//#include "AImagician/AIMagic.h"
//#include "AIMagic_Attack.generated.h"

/**
 * 
 */

//advise version - not the solution
/*
UCLASS()
class SHADOWRUNNER_API UAIMagic_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UAIMagic_Attack(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool montage_has_finished(AAIMagic* const npc);

	class AAIMagicController* cont;
	class UBlackboardComponent* blackboardComponent;
	class AAIMagic* magician;
};
*/

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ShadowRunner/ShadowRunnerCharacter.h"
#include "AImagician/AIMagic.h"
#include "AIMagic_Attack.generated.h"

/**
 *
 */
UCLASS()
class SHADOWRUNNER_API UAIMagic_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UAIMagic_Attack(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	bool montage_has_finished(AAIMagic* const npc);
};
