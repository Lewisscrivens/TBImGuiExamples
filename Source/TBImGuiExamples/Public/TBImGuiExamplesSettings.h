// Copyright Lewis Scrivens. All Rights Reserved.

#pragma once

#include <Engine/DeveloperSettings.h>

#include <TBImGuiTypes.h>

#include "TBImGuiExamplesSettings.generated.h"

/// @brief TBImGui plugin settings to control the flow of ImGui within the project.
UCLASS(MinimalAPI, Config="Game", DefaultConfig, meta = (DisplayName = "ImGui Toolbox Exampels"))
class UTBImGuiExamplesSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	/// @brief Returns the singleton settings object.
	static const UTBImGuiExamplesSettings* Get()
	{
		return GetDefault<UTBImGuiExamplesSettings>();
	}
	
	/// Should the ImGui viewport content be shown on game start?
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	bool bShowImGuiExamplesOnStartup = true;
	
	/// The input mode the context should use when the ImGui content is visible.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	ETBImGuiInputMode InputModeEnabled = ETBImGuiInputMode::All;
	
	/// The input mode the context should use when the ImGui content is hidden.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	ETBImGuiInputMode InputModeDisabled = ETBImGuiInputMode::ViewportsOnly;
};