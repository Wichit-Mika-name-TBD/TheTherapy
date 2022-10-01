// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TheTherapy : ModuleRules
{
	public TheTherapy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara" });

        PrivateDependencyModuleNames.AddRange(new string[] { "QofL" });
    }
}
