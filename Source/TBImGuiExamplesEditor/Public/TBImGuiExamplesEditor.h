// Copyright Lewis Scrivens. All Rights Reserved.

#pragma once

#include <Modules/ModuleManager.h>

#include <TBImGuiContext.h>
#include <Slate/SImGuiEditorWindow.h>

class SDockTab;
class FSpawnTabArgs;

/// @brief ImGui editor example content.
/// 
/// There are two examples hosted in this module:
///  - A slate-based window that can draw ImGui content within itself.
///  - A slate-based editor docking tab that draws ImGui content within itself.
///  - A menu entry that can toggle the ImGui demo content on/off within the editor's ImGui mainframe content.
///  
class FTBImGuiExamplesEditorModule : public IModuleInterface
{
public:

	/// Begin IModuleInterface Override
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	/// End IModuleInterface Override
	
protected:

	/// @brief Registers the ImGui example menu in the editor.
	void RegisterImGuiExampleMenu();
	
#pragma region Slate Windows Example
	
	/// @brief Open a slate-based ImGui demo window in the editor.
	void OpenExampleSlateWindow();
	
	/// @brief Register two example docking tabs.
	void RegisterExampleDockingTabs();
	
	static TSharedRef<SDockTab> SpawnImGuiDemoTab(const FSpawnTabArgs& SpawnTabArgs);
	static void DrawImGuiDemoContents();
	
#if WITH_IMPLOT
	static TSharedRef<SDockTab> SpawnImPlotDemoTab(const FSpawnTabArgs& SpawnTabArgs);
	static void DrawImPlotDemoContents();
#endif
	
#if WITH_IMPLOT3D
	static TSharedRef<SDockTab> SpawnImPlot3DDemoTab(const FSpawnTabArgs& SpawnTabArgs);
	static void DrawImPlot3DDemoContents();
#endif
	
	TWeakPtr<SImGuiEditorWindow> ImGuiExampleWindowWeak = nullptr;
	
#pragma endregion
	
#pragma region Mainframe Example
	
	/// @brief Ran when the editor context is fully initialized.
	void OnImGuiEditorContextCreated(const TSharedPtr<FTBImGuiContext> InContext);
	
	/// @brief Ran each ImGui tick to draw example content in the editor context.
	void DrawExampleContent();
	
	bool bShowDemoWindow = false;
	
	FDelegateHandle ImGuiEditorContextCreatedHandle;
	
#pragma endregion
	
	FDelegateHandle ToolMenusStartupHandle;
};