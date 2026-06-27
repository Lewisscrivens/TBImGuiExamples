// Copyright Lewis Scrivens. All Rights Reserved.

#include "TBImGuiExamples/Public/TBImGuiExamplesSubsystem.h"

#include <Engine/World.h>
#include <Engine/GameInstance.h>

THIRD_PARTY_INCLUDES_START
#include <imgui.h>
#if WITH_NETIMGUI
#include <NetImgui_Api.h>
#endif
THIRD_PARTY_INCLUDES_END

#include <TBImGuiTypes.h>
#include <TBImGuiSettings.h>

#include "TBImGuiExamplesInput.h"
#include "TBImGuiExamplesSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogTBImGuiExamplesSubsystem, Log, All);

static FAutoConsoleCommandWithWorld ToggleImGui(
	TEXT("TB.ImGuiExamples.ToggleImGuiContent"),
	TEXT("Toggle the ImGui visibility and input state for the ImGui examples content."),
	FConsoleCommandWithWorldDelegate::CreateLambda([](UWorld* World)
	{
		if (!IsValid(World))
		{
			return;
		}
		
		const UGameInstance* const GameInstance = World->GetGameInstance();
		if (!IsValid(GameInstance))
		{
			return;
		}
		
#if WITH_NETIMGUI
		// When NetImGui has control of ImGui content on the active instance, disable this command.
		if (const UTBImGuiSubsystem* const ImGuiSubsystem = GetValid(GameInstance->GetSubsystem<UTBImGuiSubsystem>()))
		{
			if (ImGuiSubsystem->IsHostConnected())
			{
				return;
			}
		}
#endif
		
		if (UTBImGuiExamplesSubsystem* const ImGuiExamplesSubsystem = GetValid(GameInstance->GetSubsystem<UTBImGuiExamplesSubsystem>()))
		{
			ImGuiExamplesSubsystem->ToggleImGuiVisibility();
		}
	})
);

bool UTBImGuiExamplesSubsystem::ShouldCreateSubsystem(UObject* InOuter) const
{
	const UWorld* const CurrentWorld = InOuter->GetWorld();
	if (!IsValid(CurrentWorld))
	{
		return false;
	}
	
	const UTBImGuiSettings* const ImGuiSettings = UTBImGuiSettings::Get();
	if (!IsValid(ImGuiSettings) || !ImGuiSettings->ShouldCreateContext(CurrentWorld->GetNetMode()))
	{
		UE_LOG(LogTBImGuiExamplesSubsystem, Log, TEXT("ImGui viewport context is disabled via settings, Skipping ImGui examples set-up..."));
		return false;
	}

	return CurrentWorld->WorldType == EWorldType::PIE || CurrentWorld->WorldType == EWorldType::Game;
}

void UTBImGuiExamplesSubsystem::Initialize(FSubsystemCollectionBase& InCollection)
{
	ImGuiSubsystemWeak = InCollection.InitializeDependency<UTBImGuiSubsystem>();
	if (!ImGuiSubsystemWeak.IsValid() || !ImGuiSubsystemWeak->HasContext())
	{
		UE_LOG(LogTBImGuiExamplesSubsystem, Warning, TEXT("UTBImGuiSubsystem could not be initialized! Cannot initialize UTBImGuiExamplesSubsystem!"));
		return;
	}
	
	UGameInstance* const GameInstance = GetGameInstance();
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTBImGuiExamplesSubsystem, Warning, TEXT("Cannot find a valid game instance! Cannot initialize UTBImGuiExamplesSubsystem!"));
		return;
	}
	
	UWorld* const CurrentWorld = GetWorld();
	if (!IsValid(CurrentWorld))
	{
		return;
	}
	
	// Create the menu bar that will host all the example content.
	ExampleMenuBar = NewObject<UTBImGuiExamplesMenuBar>(this);
	ExampleMenuBar->Initialize();
	
	// Bind to the ImGui context's pre-render event to draw our example ImGui content every frame.
	FTBImGuiContextScope ContextScope(GameInstance);
	if (ContextScope.IsValid())
	{
		ContextScope->OnPreRender().AddUObject(this, &UTBImGuiExamplesSubsystem::OnImGuiFrame);
	}
	
	// Set initial example content visibility.
	const UTBImGuiExamplesSettings* const ImGuiExamplesSettings = UTBImGuiExamplesSettings::Get();
	if (IsValid(ImGuiExamplesSettings))
	{
		SetImGuiVisibility(ImGuiExamplesSettings->bShowImGuiExamplesOnStartup);
	}
}

void UTBImGuiExamplesSubsystem::ToggleImGuiVisibility()
{
	SetImGuiVisibility(!bShowExampleContent);
}

void UTBImGuiExamplesSubsystem::SetImGuiVisibility(bool bIsVisible, bool bUpdateInput)
{
	if (bShowExampleContent == bIsVisible)
	{
		return;
	}
	
	bShowExampleContent = bIsVisible;
	
	if (bUpdateInput)
	{
		TryUpdateImGuiInputMode();
	}
}

void UTBImGuiExamplesSubsystem::TryUpdateImGuiInputMode()
{
	UWorld* const CurrentWorld = GetWorld();
	if (!IsValid(CurrentWorld) || CurrentWorld->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}
	
	APlayerController* const LocalPlayerController = CurrentWorld->GetFirstPlayerController();
	if (!IsValid(LocalPlayerController))
	{
		return;
	}
	
	FTBImGuiContextScope ImGuiContext;
	if (!ImGuiContext.IsValid())
	{
		UE_LOG(LogTBImGuiExamplesSubsystem, Warning, TEXT("Failed to find runtime ImGui viewport context! Cannot apply update ImGui input mode!"));
		return;
	}
	
	const UTBImGuiExamplesSettings* const ImGuiExamplesSettings = UTBImGuiExamplesSettings::Get();
	if (!ensure(IsValid(ImGuiExamplesSettings)))
	{
		return;
	}
	
	if (bShowExampleContent)
	{
		LocalPlayerController->SetShowMouseCursor(true);
		LocalPlayerController->SetInputMode(FTBImGuiExamplesInputMode());
	}
	else
	{
		LocalPlayerController->SetShowMouseCursor(false);
		LocalPlayerController->SetInputMode(FInputModeGameOnly());
	}
	
	ImGuiContext->SetInputMode(bShowExampleContent ? ImGuiExamplesSettings->InputModeEnabled : ImGuiExamplesSettings->InputModeDisabled);
}

void UTBImGuiExamplesSubsystem::OnImGuiFrame()
{
	if (!ensureMsgf(IsValid(ExampleMenuBar), TEXT("Cannot render example content without the menu bar!")))
	{
		return;
	}
	
#if WITH_NETIMGUI 
	UpdateNetImGuiVisibility();
#endif
	
	// Only show example menu bar when example content is visible.
	if (bShowExampleContent)
	{
		// Example docking space in viewport.
		ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode 
			| ImGuiDockNodeFlags_NoDockingOverCentralNode);
		
		// Toggle the visibility of the ImGui examples content with F1 or 
		// gamepad special left if ImGui has input focus.
		if (ImGui::IsKeyPressed(ImGui::ToImGuiKey(EKeys::F1)) || 
			ImGui::IsKeyPressed(ImGui::ToImGuiKey(EKeys::Gamepad_Special_Left)))
		{
			SetImGuiVisibility(false);
		}
		
		ExampleMenuBar->DrawMenuBar();
	}
	
	// Always draw windows so we can still see the content while playing.
	ExampleMenuBar->DrawWindows();
}

#if WITH_NETIMGUI
void UTBImGuiExamplesSubsystem::UpdateNetImGuiVisibility()
{
	if (!ImGuiSubsystemWeak.IsValid())
	{
		return;
	}
	
	if (bShowExampleContent)
	{
		// When NetImGui is disconnected, update the visibility.
		if (bVisibleFromConnection && !ImGuiSubsystemWeak->IsHostConnected())
		{
			SetImGuiVisibility(false, false);
			bVisibleFromConnection = false;
		}
	}
	else
	{
		// When NetImGui is first connected, update the visibility for NetImGui connection.
		if (!bVisibleFromConnection && ImGuiSubsystemWeak->IsHostConnected())
		{
			SetImGuiVisibility(true, false);
			bVisibleFromConnection = true;
		}
	}
}
#endif