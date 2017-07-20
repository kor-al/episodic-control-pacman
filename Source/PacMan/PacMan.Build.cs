// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class PacMan : ModuleRules
{
	public PacMan(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RenderCore", "ShaderCore", "OpenCV"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PublicAdditionalLibraries.Add(@"D:/Alice/Documents/Unreal Projects/Pac Man 4.15/Binaries/Win64/opencvtest.lib");

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
