// Copyright Lewis Scrivens. All Rights Reserved.

#pragma once

#include <Subsystems/GameInstanceSubsystem.h>

#include <TBImGuiSubsystem.h>

#include "TBImGuiExamplesMenuBar.h"

#include "TBImGuiExamplesSubsystem.generated.h"

/// @brief ImGui runtime example content.
/// 
/// This subsystem is created game instance / game viewport and hosts the following examples:
///  - Binding to the ImGui viewport context and drawing ImGui content within the game viewport.
///  - ImGui menu bar being drawn within the game viewport context.
///  - Implementation of ImGui and ImPlots demo / examples windows registered onto the menu bar.
///  - List of all actors in the game world rendered in an ImGui window.
///  - Unreal engine texture rendered in an ImGui window.
///  
///  These are registered with an example menu bar and window system.
///  
UCLASS(MinimalAPI)
class UTBImGuiExamplesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	friend class UTBImGuiExamplesLocalPlayerSubsystem;
	
public:
	
	/// Begin USubsystem Override
	virtual bool ShouldCreateSubsystem(UObject* InOuter) const override;
	virtual void Initialize(FSubsystemCollectionBase& InCollection) override;
	/// End USubsystem Override

	/// @brief Show/Hide example content.
	void ToggleImGuiVisibility();
	void SetImGuiVisibility(bool bIsVisible, bool bUpdateInput = true);
	
protected:
	
	/// @brief Update ImGui example input state based off visibility.
	void TryUpdateImGuiInputMode();
	
	/// @brief Called when ImGui is drawing a new frame.
	void OnImGuiFrame();
	
#if WITH_NETIMGUI
	/// @brief Update menubar visibility for active NetImGui connections.
	void UpdateNetImGuiVisibility();
	bool bVisibleFromConnection = false;
#endif
	
	UPROPERTY(Transient)
	TObjectPtr<UTBImGuiExamplesMenuBar> ExampleMenuBar = nullptr;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UTBImGuiSubsystem> ImGuiSubsystemWeak = nullptr;
	
	bool bShowExampleContent = false;
};