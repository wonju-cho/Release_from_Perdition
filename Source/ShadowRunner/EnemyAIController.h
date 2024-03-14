#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"


UCLASS()
class SHADOWRUNNER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());

	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;

	// Adam's modifications
	void OnEnemyDeath();
	//------------------------
	class UBlackboardComponent* GetBlackBoard() const;


private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
		class UBehaviorTreeComponent* BehaviorTreeComponent;

	class UBlackboardComponent* BlackBoard;

	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Hearing* HearingConfig;

	UFUNCTION()
	void OnUpdated(TArray<AActor*> const& updatedActors);

	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);

	void SetupPerceptionSystem();

	// Adam's modifications
	APawn* controlledPawn;
	//-----------------------

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float SightRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float HearingRange;
	

};
