// Copyright Lewis Scrivens. All Rights Reserved.

#pragma once

#include <Subsystems/LocalPlayerSubsystem.h>

#include "TBImGuiExamplesLocalPlayerSubsystem.generated.h"

UCLASS(MinimalAPI)
class UTBImGuiExamplesLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	
	/// Begin ULocalPlayerSubsystem Override
	virtual void PlayerControllerChanged(APlayerController* InPlayerController) override;
	/// End ULocalPlayerSubsystem Override

protected:
	
	/// @brief Initialize input command for toggling the ImGui examples content.
	void InitializeImGuiCommand(APlayerController* InPlayerController);
};