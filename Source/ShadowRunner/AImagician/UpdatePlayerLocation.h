// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "UpdatePlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UUpdatePlayerLocation : public UBTService
{
	GENERATED_BODY()
public:
	UUpdatePlayerLocation();
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float dt) override;

};
