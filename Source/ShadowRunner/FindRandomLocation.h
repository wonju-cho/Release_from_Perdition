// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UFindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY(Blueprintable)
	
public:
	UFindRandomLocation(FObjectInitializer const& ObjectInitializer);
	
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowAPrivateAccess = "true"))
	float SearchRadius = 1500.f;

};
