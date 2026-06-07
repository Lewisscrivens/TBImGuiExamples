// Copyright Lewis Scrivens. All Rights Reserved.

#include "TBImGuiExamplesLocalPlayerSubsystem.h"

#include <GameFramework/PlayerInput.h>

#include "TBImGuiExamplesSubsystem.h"

class UTBImGuiExamplesSubsystem;
DEFINE_LOG_CATEGORY_STATIC(LogTBImGuiLocalPlayerSubsystem, Log, All);

void UTBImGuiExamplesLocalPlayerSubsystem::PlayerControllerChanged(APlayerController* InPlayerController)
{
	UWorld* const CurrentWorld = GetWorld();
	if (!IsValid(CurrentWorld))
	{
		UE_LOG(LogTBImGuiLocalPlayerSubsystem, Warning, TEXT("Invalid World! Cannot setup ImGui input for the local player!"));
		return;
	}
	
	const UGameInstance* const GameInstance = CurrentWorld->GetGameInstance();
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTBImGuiLocalPlayerSubsystem, Warning, TEXT("Invalid Game Instance! Cannot setup ImGui input for the local player!"));
		return;
	}
	
	// Bind commands for keyboard and gamepad for opening the example content with F1 and gamepad special left.
	InitializeImGuiCommand(InPlayerController);
	
	// Update input state since our player controller just finished initializing.
	if (UTBImGuiExamplesSubsystem* const ImGuiExamplesSubsystem = GetValid(GameInstance->GetSubsystem<UTBImGuiExamplesSubsystem>()))
	{
		ImGuiExamplesSubsystem->TryUpdateImGuiInputMode();
	}
}

void UTBImGuiExamplesLocalPlayerSubsystem::InitializeImGuiCommand(APlayerController* InPlayerController)
{
	UPlayerInput* const PlayerInput = InPlayerController->PlayerInput;
	if (!IsValid(PlayerInput))
	{
		UE_LOG(LogTBImGuiLocalPlayerSubsystem, Warning, TEXT("Invalid Player Input! Cannot setup ImGui input for the local player!"));
		return;
	}
	
	auto AlreadyBoundToKey = [&PlayerInput] (const FKey& InKey)
	{
		for (FKeyBind& BoundKey : PlayerInput->DebugExecBindings)
		{
			if (BoundKey.Key == InKey && BoundKey.Command == TEXT("TB.ImGuiExamples.ToggleImGuiContent"))
			{
				return true;
			}
		}
		
		return false;
	};
	
	if (!AlreadyBoundToKey(EKeys::F1))
	{
		FKeyBind KeyBind;
		KeyBind.Key = EKeys::F1;
		KeyBind.Command = TEXT("TB.ImGuiExamples.ToggleImGuiContent");
		PlayerInput->DebugExecBindings.Add(KeyBind);
	}
	
	if (!AlreadyBoundToKey(EKeys::Gamepad_Special_Left))
	{
		FKeyBind KeyBind;
		KeyBind.Key = EKeys::Gamepad_Special_Left;
		KeyBind.Command = TEXT("TB.ImGuiExamples.ToggleImGuiContent");
		PlayerInput->DebugExecBindings.Add(KeyBind);
	}
}