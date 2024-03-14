// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "IsShadowInMeleeRange.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UIsShadowInMeleeRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UIsShadowInMeleeRange();

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float MeleeRange = 25.0f;

};
