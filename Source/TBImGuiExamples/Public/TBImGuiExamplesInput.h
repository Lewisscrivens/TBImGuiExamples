// Copyright Lewis Scrivens. All Rights Reserved.

#pragma once

#include <GameFramework/PlayerController.h>

/// @brief Input mode for runtime viewports to interact with both the game and ImGui at the same time.
struct FTBImGuiExamplesInputMode : public FInputModeDataBase
{
	/// Begin FInputModeDataBase Override
#if UE_ENABLE_DEBUG_DRAWING
	virtual const FString& GetDebugDisplayName() const override;
#endif
	
protected:
	virtual void ApplyInputMode(FReply& SlateOperations, UGameViewportClient& GameViewportClient) const override;
	/// End FInputModeDataBase Override
};