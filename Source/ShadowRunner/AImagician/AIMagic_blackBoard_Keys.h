#pragma once

#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

namespace magic_bb_keys
{
	TCHAR const* const target_location = TEXT("TargetLocation");
	TCHAR const* const can_see_player = TEXT("CanSeePlayer");
	TCHAR const* const player_is_in_magic_range = TEXT("PlayerIsInMagicRange");
	TCHAR const* const player_location = TEXT("PlayerLocation");
	TCHAR const* const shadow_location = TEXT("ShadowLocation");
	TCHAR const* const shadow_target_location = TEXT("ShadowTargetLocation");
	TCHAR const* const can_see_shadow = TEXT("CanSeeShadow");
	TCHAR const* const shadow_is_in_magic_range = TEXT("ShadowIsInMagicRange");
	TCHAR const* const got_attack_behind = TEXT("GotAttackBehind");
	TCHAR const* const is_on_cylinder = TEXT("IsOnCylinder");
}
