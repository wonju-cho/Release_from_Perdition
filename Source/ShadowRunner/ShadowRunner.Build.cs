// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShadowRunner : ModuleRules
{
	public ShadowRunner(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "Slate", "SlateCore" });
		PublicDependencyModuleNames.AddRange(new string[] 
		{ "Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"AIModule",
			"GameplayTasks",
			"NavigationSystem",
			"UMG",
			"Slate",
			"SlateCore",
		"Niagara", "ApexDestruction"
        });
	}
}
