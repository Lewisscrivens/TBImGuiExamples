// Copyright Lewis Scrivens. All Rights Reserved.

#include "TBImGuiExamplesEditor.h"

#include "TBImGuiDemo.h"

THIRD_PARTY_INCLUDES_START
#include <imgui.h>
#if WITH_IMPLOT
#include <implot.h>
#endif
#if WITH_IMPLOT3D
#include <implot3d.h>
#endif
THIRD_PARTY_INCLUDES_END

#include <TBImGuiEditor.h>
#include <TBImGuiEditorSettings.h>
#include <Slate/SImGuiContextWidget.h>

IMPLEMENT_MODULE(FTBImGuiExamplesEditorModule, TBImGuiExamplesEditor)

DEFINE_LOG_CATEGORY_STATIC(LogTBImGuiExamplesEditor, Log, All);

static const FName ImGuiDemoTabName(TEXT("ImGuiDemoTab"));

#if WITH_IMPLOT
static const FName ImPlotDemoTabName(TEXT("ImPlotDemoTab"));
#endif

#if WITH_IMPLOT3D
static const FName ImPlot3DDemoTabName(TEXT("ImPlot3DDemoTab"));
#endif

#define LOCTEXT_NAMESPACE "FTBImGuiExamplesModule"

void FTBImGuiExamplesEditorModule::StartupModule()
{
	const UTBImGuiEditorSettings* const ImGuiEditorSettings = UTBImGuiEditorSettings::Get();
	if (!IsValid(ImGuiEditorSettings) || !ImGuiEditorSettings->bCreateEditorContext)
	{
		UE_LOG(LogTBImGuiExamplesEditor, Log, TEXT("ImGui context is disabled via settings for the editor, Skipping ImGui editor examples set-up..."));
		return;
	}
	
	// Register binding to ImGui editor drawing.
	if (FTBImGuiEditorModule::HasEditorContext())
	{
		OnImGuiEditorContextCreated(FTBImGuiEditorModule::GetEditorContext());
	}
	else
	{
		FTBImGuiEditorModule::OnContextCreated.AddRaw(this, &FTBImGuiExamplesEditorModule::OnImGuiEditorContextCreated);
	}
	
	// Register example tool menu entries.
	ToolMenusStartupHandle = UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::
		CreateRaw(this, &FTBImGuiExamplesEditorModule::RegisterImGuiExampleMenu));
	
	// Register example ImGui docking tabs.
	RegisterExampleDockingTabs();
}

void FTBImGuiExamplesEditorModule::ShutdownModule()
{
	// Unregister example tool menu entry.
	if (ToolMenusStartupHandle.IsValid())
	{
		UToolMenus::UnRegisterStartupCallback(ToolMenusStartupHandle);
	}
	
	// Unregister example tab spawner.
	if (FSlateApplication::IsInitialized())
	{
		FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ImGuiDemoTabName);
	}
}

void FTBImGuiExamplesEditorModule::RegisterImGuiExampleMenu()
{
	UToolMenus* const ToolMenus = UToolMenus::Get();
	if (!ensureAlwaysMsgf(ToolMenus, TEXT("Registry was deferred but UToolMenus is not available?!")))
	{
		return;
	}
	
	UToolMenu* const ToolsMenu = ToolMenus->FindMenu("LevelEditor.MainMenu.Tools");
	check(ToolsMenu);
	{
		FToolMenuSection& ImGuiSection = ToolsMenu->FindOrAddSection(TEXT("ImGui"), LOCTEXT("ImGuiSectionName", "ImGui"));
		
		auto ExampleMenuContent = FNewToolMenuDelegate::CreateLambda([this](UToolMenu* InMenu)
		{
			FToolMenuSection& ContentMenu = InMenu->FindOrAddSection(TEXT("ImGuiContent"), LOCTEXT("ImGuiContentLabel", "ImGui Content"));
			
			// Add a spawnable ImGui window that draws the demo content.
			ContentMenu.AddMenuEntry(
				TEXT("SlateExample"),
				LOCTEXT("SlateExampleLabel", "Slate ImGui Window"),
				LOCTEXT("SlateExampleTooltip", "Open a Slate window containing TBImGui demo content."),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("Icons.Layout")),
				FUIAction(FExecuteAction::CreateLambda([&]()
				{
					OpenExampleSlateWindow();
				}))
			);
			
			// Add a spawnable ImGui window that draws the demo content.
			ContentMenu.AddMenuEntry(
				TEXT("ImGuiExample"),
				LOCTEXT("ImGuiExampleLabel", "ImGui Demo Viewport"),
				LOCTEXT("ImGuiExampleTooltip", "Open an ImGui based window showing the ImGui demo content."),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("Icons.Layout")),
				FUIAction(FExecuteAction::CreateLambda([&]()
				{
					bShowDemoWindow = true;
				}))
			);
				
			// Add ImGui demo tab.
			ContentMenu.AddMenuEntry(
				TEXT("ImGuiDemoTab"),
				LOCTEXT("ImGuiDemoTabLabel", "ImGui Demo Tab"),
				LOCTEXT("ImGuiDemoTabTooltip", "Spawn ImGui demo docking tab."),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("Icons.Layout")),
				FUIAction(FExecuteAction::CreateLambda([]
				{
					FGlobalTabmanager::Get()->TryInvokeTab(ImGuiDemoTabName);
				})));
				
#if WITH_IMPLOT
			// Add ImPlot demo tab.
			ContentMenu.AddMenuEntry(
				TEXT("ImPlotDemoTab"),
				LOCTEXT("ImPlotDemoTabLabel", "ImPlot Demo Tab"),
				LOCTEXT("ImPlotDemoTabTooltip", "Spawn ImPlot demo docking tab."),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("Icons.Layout")),
				FUIAction(FExecuteAction::CreateLambda([]
				{
					FGlobalTabmanager::Get()->TryInvokeTab(ImPlotDemoTabName);
				})));
#endif
			
#if WITH_IMPLOT3D
			// Add ImPlot3D demo tab.
			ContentMenu.AddMenuEntry(
				TEXT("ImPlot3DDemoTab"),
				LOCTEXT("ImPlot3DDemoTabLabel", "ImPlot3D Demo Tab"),
				LOCTEXT("ImPlot3DDemoTabTooltip", "Spawn ImPlot3D demo docking tab."),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("Icons.Layout")),
				FUIAction(FExecuteAction::CreateLambda([]
				{
					FGlobalTabmanager::Get()->TryInvokeTab(ImPlot3DDemoTabName);
				})));
#endif
		});
		
		ImGuiSection.AddSubMenu(
			TEXT("ImGui Examples"), 
			LOCTEXT("ImGuiExamplesMenuLabel", "ImGui Examples"), 
			LOCTEXT("ImGuiExamplesMenuTooltip", "Open example windows and tabs showcasing ImGui features in the editor."),
			ExampleMenuContent,
			false,
			FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("ClassIcon.UserDefinedStruct")));
	}
}

#pragma region Slate Windows Example

void FTBImGuiExamplesEditorModule::OpenExampleSlateWindow()
{
	// Early out and focus the existing window if already open.
	if (ImGuiExampleWindowWeak.IsValid())
	{
		TSharedPtr<SWindow> ImGuiExampleWindow = ImGuiExampleWindowWeak.Pin();
		ImGuiExampleWindow->BringToFront();
		FSlateApplication::Get().SetUserFocus(0, ImGuiExampleWindow, EFocusCause::SetDirectly);
		return;
	}
	
	// Create a new slate window.
	TSharedRef<SImGuiEditorWindow> ExampleWindow = SAssignNew(ImGuiExampleWindowWeak, SImGuiEditorWindow)
		.Title(LOCTEXT("ImGuiDemoWindowTitle", "ImGui Example Window"))
		.DrawContent(FSimpleDelegate::CreateLambda([]
		{
			const ImGuiID MainViewportDockSpaceId = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar);
			ImGui::SetNextWindowDockID(MainViewportDockSpaceId);
			FTBImGuiDemo::ShowDemoWindow();
		}));
	
	// Instance the window.
	FSlateApplication::Get().AddWindow(ExampleWindow);
}

void FTBImGuiExamplesEditorModule::RegisterExampleDockingTabs()
{
	// Add ImGui demo example tab.
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ImGuiDemoTabName, 
		FOnSpawnTab::CreateStatic(&FTBImGuiExamplesEditorModule::SpawnImGuiDemoTab))
		.SetDisplayName(LOCTEXT("DisplayName_ImGuiDemoTab", "ImGui Demo"))
		.SetTooltipText(LOCTEXT("Tooltip_ImGuiDemoTab", "ImGui demo content rendered in an editor docking tab."))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
	
#if WITH_IMPLOT
	// Add ImPlot demo example tab.
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ImPlotDemoTabName, 
		FOnSpawnTab::CreateStatic(&FTBImGuiExamplesEditorModule::SpawnImPlotDemoTab))
		.SetDisplayName(LOCTEXT("DisplayName_ImPlotDemoTab", "ImPlot Demo"))
		.SetTooltipText(LOCTEXT("Tooltip_ImPlotDemoTab", "ImPlot demo content rendered in an editor docking tab."))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
#endif
	
#if WITH_IMPLOT3D
	// Add ImPlot demo example tab.
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ImPlot3DDemoTabName, 
		FOnSpawnTab::CreateStatic(&FTBImGuiExamplesEditorModule::SpawnImPlot3DDemoTab))
		.SetDisplayName(LOCTEXT("DisplayName_ImPlot3DDemoTab", "ImPlot3D Demo"))
		.SetTooltipText(LOCTEXT("Tooltip_ImPlot3DDemoTab", "ImPlot3D demo content rendered in an editor docking tab."))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
#endif
}

TSharedRef<SDockTab> FTBImGuiExamplesEditorModule::SpawnImGuiDemoTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FTBImGuiContextParams ContextParams;
	ContextParams.ContextName = TEXT("ImDemoContext");
	ContextParams.ViewportMode = ETBImGuiViewportMode::Single;
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SImGuiContextWidget)
				.ContextParams(ContextParams)
				.OwningWindow(SpawnTabArgs.GetOwnerWindow())
				.DrawContent(FSimpleDelegate::CreateStatic(DrawImGuiDemoContents))
		];
}

void FTBImGuiExamplesEditorModule::DrawImGuiDemoContents()
{
	const ImGuiID MainViewportDockSpaceId = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar);
	ImGui::SetNextWindowDockID(MainViewportDockSpaceId);
	ImGui::ShowDemoWindow(nullptr);
}

#if WITH_IMPLOT
TSharedRef<SDockTab> FTBImGuiExamplesEditorModule::SpawnImPlotDemoTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FTBImGuiContextParams ContextParams;
	ContextParams.ContextName = TEXT("ImPlotContext");
	ContextParams.ViewportMode = ETBImGuiViewportMode::Single;
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SImGuiContextWidget)
				.ContextParams(ContextParams)
				.OwningWindow(SpawnTabArgs.GetOwnerWindow())
				.DrawContent(FSimpleDelegate::CreateStatic(DrawImPlotDemoContents))
		];
}

void FTBImGuiExamplesEditorModule::DrawImPlotDemoContents()
{
	const ImGuiID MainViewportDockSpaceId = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar);
	ImGui::SetNextWindowDockID(MainViewportDockSpaceId);
	ImPlot::ShowDemoWindow(nullptr);
}
#endif

#if WITH_IMPLOT3D
TSharedRef<SDockTab> FTBImGuiExamplesEditorModule::SpawnImPlot3DDemoTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FTBImGuiContextParams ContextParams;
	ContextParams.ContextName = TEXT("ImPlot3DContext");
	ContextParams.ViewportMode = ETBImGuiViewportMode::Single;
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SImGuiContextWidget)
				.ContextParams(ContextParams)
				.OwningWindow(SpawnTabArgs.GetOwnerWindow())
				.DrawContent(FSimpleDelegate::CreateStatic(DrawImPlot3DDemoContents))
		];
}

void FTBImGuiExamplesEditorModule::DrawImPlot3DDemoContents()
{
	const ImGuiID MainViewportDockSpaceId = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar);
	ImGui::SetNextWindowDockID(MainViewportDockSpaceId);
	ImPlot3D::ShowDemoWindow(nullptr);
}
#endif

#pragma endregion 

#pragma region Mainframe Example

void FTBImGuiExamplesEditorModule::OnImGuiEditorContextCreated(const TSharedPtr<FTBImGuiContext> InContext)
{
	InContext->BindPreRender(FSimpleMulticastDelegate::FDelegate::CreateRaw
		(this, &FTBImGuiExamplesEditorModule::DrawExampleContent));
}

void FTBImGuiExamplesEditorModule::DrawExampleContent()
{
	if (bShowDemoWindow)
	{
		ImGui::ShowDemoWindow(&bShowDemoWindow);
	}
}

#pragma endregion

#undef LOCTEXT_NAMESPACE