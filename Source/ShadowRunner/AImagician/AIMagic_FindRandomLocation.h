// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "AIMagic_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SHADOWRUNNER_API UAIMagic_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UAIMagic_FindRandomLocation(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory);
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
		float search_radius = 800.0f;

};
