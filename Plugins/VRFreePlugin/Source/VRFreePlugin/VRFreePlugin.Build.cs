// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;


public class VRFreePlugin : ModuleRules
{




    public VRFreePlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"VRFreePlugin/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"VRFreePlugin/Private",
				// ... add other private include paths required here ...
			}
			);

        PrivateIncludePathModuleNames.AddRange(
             new string[]
             {
                    "InputDevice",			// For IInputDevice.h
					"HeadMountedDisplay",	// For IMotionController.h
					"ImageWrapper"
             });


        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "InputCore",
                "HeadMountedDisplay",
                "ApplicationCore",
                "Projects",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
        string PluginDirectory = Path.Combine(BaseDirectory, "ThirdParty", "VRfree", Target.Platform.ToString());

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            RuntimeDependencies.Add(Path.Combine(PluginDirectory, "VRfreeDotNET.dll"));
            RuntimeDependencies.Add(Path.Combine(PluginDirectory, "VRfreeBasicTypesDotNetFramework.dll"));
            PublicAdditionalLibraries.Add(Path.Combine(PluginDirectory, "VRfreeC.lib"));
            RuntimeDependencies.Add(Path.Combine(PluginDirectory, "VRfreeC.dll"));
            RuntimeDependencies.Add(Path.Combine(PluginDirectory, "Newtonsoft.Json.dll"));
        }
    }
}
