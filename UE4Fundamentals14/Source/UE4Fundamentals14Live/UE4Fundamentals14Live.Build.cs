// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE4Fundamentals14Live : ModuleRules
{
	public UE4Fundamentals14Live(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
