#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ShadowRunnerCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BlackBoardKeys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "ShadowRunnerCharacter.h"
#include "ShadowCloneCharacter.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "AITags.h"
#include "Music_Interface.h"
#include "GameFramework/GameState.h"
#include "Misc/OutputDeviceDebug.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> Object(TEXT("BehaviorTree'/Game/BluePrints/Enemy/EnemyCharacter_BT.EnemyCharacter_BT'"));

	if (Object.Succeeded())
	{
		BehaviorTree = Object.Object;
	}

	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComponent"));
	BlackBoard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));

	SightRange = 3000.f;
	HearingRange = 1500.f;
	
	SetupPerceptionSystem();
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree(*BehaviorTree);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Adam's modifications 
	controlledPawn = InPawn;
	//------------------------

	if (BlackBoard)
	{
		BlackBoard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}

// Adam's modifications
void AEnemyAIController::OnEnemyDeath()
{
	bool playerDetect = GetBlackBoard()->GetValueAsBool(bbKeys::CanSeePlayer);
	if (playerDetect == true)
	{
		AGameStateBase* gameState = GetWorld()->GetGameState();
		const FString command = FString::Printf(TEXT("Update_Enemy_Count %d"), -1);
		FOutputDeviceDebug debug;
		gameState->CallFunctionByNameWithArguments(*command, debug, this, true);
	}
}
//----------------------------------------------------------------------------------------

UBlackboardComponent* AEnemyAIController::GetBlackBoard() const
{
	return BlackBoard;
}

void AEnemyAIController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	if (auto const ch = Cast<AShadowRunnerCharacter>(actor))
	{
		// Adam's modifications
		bool playerDetect = GetBlackBoard()->GetValueAsBool(bbKeys::CanSeePlayer);
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
		// -------------------------------------------
		GetBlackBoard()->SetValueAsBool(bbKeys::CanSeePlayer, stimulus.WasSuccessfullySensed());
		// Adam's modifications
		const FString command = FString::Printf(TEXT("Update_Enemy_Count %d"), enemyCount);
		
		gameState->CallFunctionByNameWithArguments(*command, debug, this, true);
		// -------------------------------------------
	}

	if(auto const ch = Cast<AShadowCloneCharacter>(actor))
	{
		GetBlackBoard()->SetValueAsBool(bbKeys::CanSeeShadow, stimulus.WasSuccessfullySensed());
	}
}

void AEnemyAIController::OnUpdated(TArray<AActor*> const& updatedActors)
{
	for(size_t x = 0; x < updatedActors.Num(); ++x)
	{
		FActorPerceptionBlueprintInfo info;
		GetPerceptionComponent()->GetActorsPerception(updatedActors[x], info);
		for(size_t j = 0; j < info.LastSensedStimuli.Num(); ++j)
		{
			FAIStimulus const Stimulus = info.LastSensedStimuli[j];
			if(Stimulus.Tag == tags::NoiseTag)
			{
				GetBlackBoard()->SetValueAsBool(bbKeys::IsInvestigating, Stimulus.WasSuccessfullySensed());
				GetBlackBoard()->SetValueAsVector(bbKeys::TargetLocation, Stimulus.StimulusLocation);
			}
			
		}
		
	}
}

void AEnemyAIController::SetupPerceptionSystem()
{
	//create and initialize sight configuration object
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if(SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = SightRange;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		//add sight configuration component to perception component
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	}

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if(HearingConfig)
	{
		HearingConfig->HearingRange = HearingRange;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnUpdated);
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
}