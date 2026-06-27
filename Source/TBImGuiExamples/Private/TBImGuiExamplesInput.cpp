// Copyright Lewis Scrivens. All Rights Reserved.

#include "TBImGuiExamplesInput.h"

#include <Input/Reply.h>
#include <Engine/GameViewportClient.h>

THIRD_PARTY_INCLUDES_START
#include <imgui.h>
THIRD_PARTY_INCLUDES_END

#include <TBImGuiTypes.h>
#include <TBImGuiUtils.h>

DEFINE_LOG_CATEGORY_STATIC(LogTBImGuiExampleInput, Log, All);

#if UE_ENABLE_DEBUG_DRAWING
const FString& FTBImGuiExamplesInputMode::GetDebugDisplayName() const
{
	static const FString DebugName = TEXT("ImGui_Examples_InputMode");
	return DebugName;
}
#endif

void FTBImGuiExamplesInputMode::ApplyInputMode(FReply& SlateOperations, UGameViewportClient& GameViewportClient) const
{
	FTBImGuiContextScope ImGuiContext;
	if (!ImGuiContext.IsValid())
	{
		UE_LOG(LogTBImGuiExampleInput, Warning, TEXT("Failed to find runtime ImGui viewport context! Cannot apply ImGui viewport input mode!"));
		return;
	}
	
	TSharedPtr<SWidget> ImGuiMainViewportWidget = FTBImGuiUtils::GetViewportWidget(ImGui::GetMainViewport());
	if (ImGuiMainViewportWidget.IsValid())
	{
		SlateOperations.SetUserFocus(ImGuiMainViewportWidget.ToSharedRef());
	}
	
	SlateOperations.ReleaseMouseLock();
	SlateOperations.ReleaseMouseCapture();
	GameViewportClient.SetMouseLockMode(EMouseLockMode::DoNotLock);
	GameViewportClient.SetMouseCaptureMode(EMouseCaptureMode::CaptureDuringMouseDown);
}