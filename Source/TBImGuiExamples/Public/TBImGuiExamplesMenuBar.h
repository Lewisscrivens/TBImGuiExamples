// Copyright Lewis Scrivens. All Rights Reserved.

#pragma once

#include <UObject/Object.h>
#include <UObject/SoftObjectPtr.h>

#include "TBImGuiExamplesWindows.h"

#include "TBImGuiExamplesMenuBar.generated.h"

USTRUCT()
struct FTbImGuiExamplesMenuBarCategory
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	TArray<TSoftClassPtr<UTBImGuiExamplesWindow_Base>> RegisteredWindows;
};

/// @brief This is an example of how you can implement a menu bar within Unreal.
/// This is purely a UObject so we can hook into the reflection system to cut down on unnecessary code.
UCLASS(MinimalAPI)
class UTBImGuiExamplesMenuBar : public UObject
{
	GENERATED_BODY()
	
public:
	
	/// @brief Find and register all window types.
	void Initialize();
	
	/// @brief Draw the menu bar and entries via ImGui.
	void DrawMenuBar();
	
	/// @brief Draw all the open ImGui windows.
	void DrawWindows();
	
	/// @brief Request a window is closed at the start of the next ImGui frame.
	void RequestCloseWindow(UTBImGuiExamplesWindow_Base* InWindow);
	
protected:
	
	UPROPERTY(Transient)
	TMap<FString, FTbImGuiExamplesMenuBarCategory> MenuBar;
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UTBImGuiExamplesWindow_Base>> OpenWindows;
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UTBImGuiExamplesWindow_Base>> CloseWindows;
};