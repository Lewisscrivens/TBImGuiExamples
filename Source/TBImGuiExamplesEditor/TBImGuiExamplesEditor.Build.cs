// Copyright Lewis Scrivens. All Rights Reserved.

using UnrealBuildTool;

public class TBImGuiExamplesEditor : ModuleRules
{
	public TBImGuiExamplesEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"Slate",
			"SlateCore",
			"DeveloperSettings",
			"TBImGui",
			"TBImGuiEditor"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"MainFrame",
			"ToolMenus"
		});
	}
}