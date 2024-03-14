#pragma once

#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Containers/UnrealString.h"

namespace bbKeys {
	TCHAR const* const TargetLocation = TEXT("TargetLocation");
	TCHAR const* const ShadowLocation = TEXT("ShadowLocation");
	TCHAR const* const CanSeePlayer = TEXT("CanSeePlayer");
	TCHAR const* const CanSeeShadow = TEXT("CanSeeShadow");
	TCHAR const* const PatrolPathVector = TEXT("PatrolPathVector");
	TCHAR const* const PatrolPathIndex = TEXT("PatrolPathIndex");
	TCHAR const* const PlayerIsInMeleeRange = TEXT("PlayerIsInMeleeRange");
	TCHAR const* const ShadowIsInMeleeRange = TEXT("ShadowIsInMeleeRange");
	TCHAR const* const IsInvestigating = TEXT("IsInvestigating");
	TCHAR const* const IsEnemyInRange = TEXT("IsEnemyInRange");
	TCHAR const* const NearLocation = TEXT("NearLocation");
	TCHAR const* const IsMeleeInCheckPointRange = TEXT("IsMeleeInCheckPointRange");
}