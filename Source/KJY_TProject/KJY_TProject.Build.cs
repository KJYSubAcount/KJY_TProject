// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KJY_TProject : ModuleRules
{
	public KJY_TProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "KJYPlugins", "OnlineSubsystem", "OnlineSubsystemUtils" });

        PublicIncludePaths.AddRange(new string[] { "KJY_TProject" });
    }
}
