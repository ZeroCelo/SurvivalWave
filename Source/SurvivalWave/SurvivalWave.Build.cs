// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SurvivalWave : ModuleRules
{
	//public SurvivalWave(TargetInfo Target)
    public SurvivalWave(ReadOnlyTargetRules Target) : base(Target)
    {
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","UMG","Slate","SlateCore", "AIModule" });
    }
}
