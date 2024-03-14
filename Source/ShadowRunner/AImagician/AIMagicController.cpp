// Fill out your copyright notice in the Description page of Project Settings.


#include "../AImagician/AIMagicController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ShadowRunner/ShadowRunnerCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "AIMagic_blackBoard_Keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "ShadowRunner/AIShadowCloneController.h"
#include "ShadowRunner/ShadowCloneCharacter.h"
#include "ShadowRunner/AImagician/AIMagic_blackBoard_Keys.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/GameState.h"
#include "Misc/OutputDeviceDebug.h"

AAIMagicController::AAIMagicController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/BluePrints/AIMagician/AIMagic_BT'"));
	
	if(obj.Succeeded())
	{
		btree = obj.Object;
	}
	Behavior_tree_component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
	setup_perception_system();
}

void AAIMagicController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(btree);
	Behavior_tree_component->StartTree(*btree);
}

void AAIMagicController::OnPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);
	if(blackboard)
	{
		blackboard->InitializeBlackboard(*btree->BlackboardAsset);
	}

	controlledPawn = InPawn;
}

void AAIMagicController::OnEnemyDeath()
{
	bool playerDetect = get_blackBoard()->GetValueAsBool(magic_bb_keys::can_see_player);
	if (playerDetect == true)
	{
		AGameStateBase* gameState = GetWorld()->GetGameState();
		const FString command = FString::Printf(TEXT("Update_Enemy_Count %d"), -1);
		FOutputDeviceDebug debug;
		gameState->CallFunctionByNameWithArguments(*command, debug, this, true);
	}
}

UBlackboardComponent* AAIMagicController::get_blackBoard() const
{
	return blackboard;
}

void AAIMagicController::on_target_detected(AActor* actor, FAIStimulus const stimulus)
{
	if(auto  const ch = Cast<AShadowRunnerCharacter>(actor))
	{
		bool playerDetect = get_blackBoard()->GetValueAsBool(magic_bb_keys::can_see_player);
		AGameStateBase* gameState = GetWorld()->GetGameState();
		FOutputDeviceDebug debug;
		int enemyCount = 0;
		if (!playerDetect && stimulus.WasSuccessfullySensed())
		{
			enemyCount = 1;
		}
		else if (playerDetect == true && !stimulus.WasSuccessfullySensed())
		{
			enemyCount = -1;
		}

		get_blackBoard()->SetValueAsBool(magic_bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
	
		const FString command = FString::Printf(TEXT("Update_Enemy_Count %d"), enemyCount);

		gameState->CallFunctionByNameWithArguments(*command, debug, this, true);
	}

	if (auto  const ch = Cast<AShadowCloneCharacter>(actor))
	{
		get_blackBoard()->SetValueAsBool(magic_bb_keys::can_see_shadow, stimulus.WasSuccessfullySensed());
	}
}

void AAIMagicController::setup_perception_system()
{
	//create and initailize sight configuration object
	sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	sight_config->SightRadius = 1500.0f;
	sight_config->LoseSightRadius = sight_config->SightRadius + 50.0f;
	sight_config->PeripheralVisionAngleDegrees = 180.0f;
	sight_config->SetMaxAge(5.0f);
	sight_config->AutoSuccessRangeFromLastSeenLocation = 900.0f;
	sight_config->DetectionByAffiliation.bDetectEnemies = true;
	sight_config->DetectionByAffiliation.bDetectFriendlies = true;
	sight_config->DetectionByAffiliation.bDetectNeutrals = true;

	//add sight configuration component to perception component
	GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIMagicController::on_target_detected);
	GetPerceptionComponent()->ConfigureSense(*sight_config);
}
