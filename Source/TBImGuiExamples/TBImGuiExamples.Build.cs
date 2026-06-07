// Copyright Lewis Scrivens. All Rights Reserved.

using UnrealBuildTool;

public class TBImGuiExamples : ModuleRules
{
	public TBImGuiExamples(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"Slate",
			"SlateCore",
			"Engine",
			"InputCore",
			"DeveloperSettings",
			"TBImGui"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"RenderCore",
			"RHI"
		});
	}
}