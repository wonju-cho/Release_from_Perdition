// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIMagicController.generated.h"
/**
 *
 */
UCLASS()
class SHADOWRUNNER_API AAIMagicController : public AAIController
{
	GENERATED_BODY()
public:
	AAIMagicController(FObjectInitializer const& object_initializer = FObjectInitializer::Get());
	void BeginPlay() override;
	void OnPossess(APawn* const InPawn) override;
	class UBlackboardComponent* get_blackBoard() const;

	void OnEnemyDeath();
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* Behavior_tree_component;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* btree;

	class UBlackboardComponent* blackboard;

	class UAISenseConfig_Sight* sight_config;

	UFUNCTION()
		void on_target_detected(AActor* actor, FAIStimulus const stimulus);

	void setup_perception_system();

	APawn* controlledPawn;

};
