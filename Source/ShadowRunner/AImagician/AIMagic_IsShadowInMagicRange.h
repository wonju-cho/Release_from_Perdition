// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "AIMagic_IsShadowInMagicRange.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UAIMagic_IsShadowInMagicRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UAIMagic_IsShadowInMagicRange();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float magic_range = 125.0f;
};
