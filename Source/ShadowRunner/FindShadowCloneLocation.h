// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindShadowCloneLocation.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UFindShadowCloneLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UFindShadowCloneLocation(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowAPrivateAccess = "true"))
		bool SearchRandom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowAPrivateAccess = "true"))
		float SearchRadius = 150.0f;

};
