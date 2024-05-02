// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Pickleball : ModuleRules
{
	public Pickleball(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Paper2D", "AIModule", "UMG", "AudioMixer", "OnlineSubsystem" });

		PrivateDependencyModuleNames.AddRange(new string[] { });
		
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemIOS");

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
