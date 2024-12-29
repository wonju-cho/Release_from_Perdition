// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShadowRunner : ModuleRules
{
	public ShadowRunner(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "Slate", "SlateCore" });
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
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
			"RHI",
    		"RenderCore",
			"Niagara", 
			"ApexDestruction"
		});

		PublicIncludePaths.AddRange(new string[] {
		    "ShadowRunner",          // ShadowRunner 기본 경로
		    "ShadowRunner/HUD",      // HUD 폴더 추가
		    "ShadowRunner/HUD/Widgets"
		});

	}
}
