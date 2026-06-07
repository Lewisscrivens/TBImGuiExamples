// Copyright Lewis Scrivens. All Rights Reserved.

#pragma once

#include <UObject/Object.h>

#include <TBImGuiTypes.h>

#include "TBImGuiExamplesWindows.generated.h"

/// @brief Simple window base class which allows us to use Unreal's reflection 
/// system to pre-register any/all ImGui windows that derive from this type.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_Base : public UObject
{
	GENERATED_BODY()
	
public:
	
	/// @brief Draw an ImGui window.
	void Draw();
	virtual void DrawWindow();
	virtual void DrawWindowContent() {}
	
	/// @brief Called when the window is first opened.
	virtual void OnWindowOpened() {}
	
	/// @brief Set this window to focus next ImGui frame.
	void FocusWindow();
	
	/// @brief Close this window.
	void CloseWindow();
	
	/// @brief Return this windows name. 
	FString GetWindowName() const;
	
	/// @brief Return the menubar category to register this window within.
	FString GetMenuCategory() const;
	
	/// @brief Returns true if the window should be registered, false otherwise.
	virtual bool ShouldRegisterWindow() const;
	
protected:
	
	FString WindowName;
	FString MenuCategory;
	FVector2D WindowSize = FVector2D(500.0f);
	
	bool bWantsFocus = false;
	bool bWindowOpen = true;
};

/// @brief ImGui demo window.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_ImGuiDemo : public UTBImGuiExamplesWindow_Base
{
	GENERATED_BODY()
	
public:
	
	UTBImGuiExamplesWindow_ImGuiDemo();
	
	/// Begin UTBImGuiExamplesWindow_Base Override
	virtual void DrawWindow() override;
	/// End UTBImGuiExamplesWindow_Base Override
};

/// @brief ImPlot demo window.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_ImPlotDemo : public UTBImGuiExamplesWindow_Base
{
	GENERATED_BODY()
	
public:
	
	UTBImGuiExamplesWindow_ImPlotDemo();
	
	/// Begin UTBImGuiExamplesWindow_Base Override
	virtual void DrawWindow() override;
	virtual bool ShouldRegisterWindow() const override;
	/// End UTBImGuiExamplesWindow_Base Override
};

/// @brief ImPlot3D demo window.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_ImPlot3DDemo : public UTBImGuiExamplesWindow_Base
{
	GENERATED_BODY()
	
public:
	
	UTBImGuiExamplesWindow_ImPlot3DDemo();
	
	/// Begin UTBImGuiExamplesWindow_Base Override
	virtual void DrawWindow() override;
	virtual bool ShouldRegisterWindow() const override;
	/// End UTBImGuiExamplesWindow_Base Override
};

/// @brief ImGui debug log window.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_ImGuiDebugLog : public UTBImGuiExamplesWindow_Base
{
	GENERATED_BODY()
	
public:
	
	UTBImGuiExamplesWindow_ImGuiDebugLog();
	
	/// Begin UTBImGuiExamplesWindow_Base Override
	virtual void DrawWindow() override;
	/// End UTBImGuiExamplesWindow_Base Override
};

/// @brief ImGui metrics window.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_ImGuiMetrics : public UTBImGuiExamplesWindow_Base
{
	GENERATED_BODY()
	
public:
	
	UTBImGuiExamplesWindow_ImGuiMetrics();
	
	/// Begin UTBImGuiExamplesWindow_Base Override
	virtual void DrawWindow() override;
	/// End UTBImGuiExamplesWindow_Base Override
};

/// @brief ImGui style editor window.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_ImGuiStyleEditor : public UTBImGuiExamplesWindow_Base
{
	GENERATED_BODY()
	
public:
	
	UTBImGuiExamplesWindow_ImGuiStyleEditor();
	
	/// Begin UTBImGuiExamplesWindow_Base Override
	virtual void DrawWindowContent() override;
	/// End UTBImGuiExamplesWindow_Base Override
};

/// @brief List of all actors in the game world rendered in an ImGui window.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_TBImGuiDemo : public UTBImGuiExamplesWindow_Base
{
	GENERATED_BODY()
	
public:
	
	UTBImGuiExamplesWindow_TBImGuiDemo();
	
	/// Begin UTBImGuiExamplesWindow_Base Override
	virtual void DrawWindow() override;
	/// End UTBImGuiExamplesWindow_Base Override
};

/// @brief List of all actors in the game world rendered in an ImGui window.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_Actors : public UTBImGuiExamplesWindow_Base
{
	GENERATED_BODY()
	
public:
	
	UTBImGuiExamplesWindow_Actors();
	
	/// Begin UTBImGuiExamplesWindow_Base Override
	virtual void OnWindowOpened() override;
	virtual void DrawWindowContent() override;
	/// End UTBImGuiExamplesWindow_Base Override
	
private:
	
	void RefreshActorList();
	void SortActorList(const int32 InColumn, bool bAscending);
	
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> ActorList;
	
	char ActorSearchFilter[256] = "";
	bool bSortActorList = false;
};

/// @brief Unreal engine texture rendered in an ImGui window.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_Texture : public UTBImGuiExamplesWindow_Base
{
	GENERATED_BODY()
	
public:
	
	UTBImGuiExamplesWindow_Texture();
	
	/// Begin UTBImGuiExamplesWindow_Base Override
	virtual void OnWindowOpened() override;
	virtual void DrawWindowContent() override;
	/// End UTBImGuiExamplesWindow_Base Override
	
private:
	
	void CreateTexture();
	
	UPROPERTY(Transient)
	TObjectPtr<UTexture2D> TestTexture = nullptr;
};

/// @brief Unreal engine unit graph rendered in an ImGui window via ImPlot.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_UnitGraph : public UTBImGuiExamplesWindow_Base
{
	GENERATED_BODY()
	
public:
	
	UTBImGuiExamplesWindow_UnitGraph();
	
	/// Begin UTBImGuiExamplesWindow_Base Override
	virtual void DrawWindowContent() override;
	virtual bool ShouldRegisterWindow() const override;
	/// End UTBImGuiExamplesWindow_Base Override
	
private:
	
	TTBImGuiRingBuffer<FVector2D> FramePoints = TTBImGuiRingBuffer<FVector2D>(2048);
	TTBImGuiRingBuffer<FVector2D> GamePoints = TTBImGuiRingBuffer<FVector2D>(2048);
	TTBImGuiRingBuffer<FVector2D> DrawPoints = TTBImGuiRingBuffer<FVector2D>(2048);
	TTBImGuiRingBuffer<FVector2D> RHITPoints = TTBImGuiRingBuffer<FVector2D>(2048);
	TTBImGuiRingBuffer<FVector2D> GPUPoints = TTBImGuiRingBuffer<FVector2D>(2048);
};

/// @brief 3D graph showing the players position and movement history via ImPlot3D.
UCLASS(MinimalAPI)
class UTBImGuiExamplesWindow_PlayerHistory : public UTBImGuiExamplesWindow_Base
{
	GENERATED_BODY()
	
public:
	
	UTBImGuiExamplesWindow_PlayerHistory();
	
	/// Begin UTBImGuiExamplesWindow_Base Override
	virtual void DrawWindowContent() override;
	virtual bool ShouldRegisterWindow() const override;
	/// End UTBImGuiExamplesWindow_Base Override

private:
	
	TTBImGuiRingBuffer<float> PlayerXPoints = TTBImGuiRingBuffer<float>(2048);
	TTBImGuiRingBuffer<float> PlayerYPoints = TTBImGuiRingBuffer<float>(2048);
	TTBImGuiRingBuffer<float> PlayerZPoints = TTBImGuiRingBuffer<float>(2048);
	
	FVector LastPlayerLocation = FVector::ZeroVector;
};