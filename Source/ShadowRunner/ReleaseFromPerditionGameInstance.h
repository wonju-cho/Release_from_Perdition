
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReleaseFromPerditionGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWRUNNER_API UReleaseFromPerditionGameInstance: public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Shutdown() override;
};
