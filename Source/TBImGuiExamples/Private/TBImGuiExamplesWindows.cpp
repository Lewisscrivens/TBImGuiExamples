// Copyright Lewis Scrivens. All Rights Reserved.

#include "TBImGuiExamplesWindows.h"

#include <EngineUtils.h>

THIRD_PARTY_INCLUDES_START
#include <imgui.h>
#if WITH_IMPLOT
#include <implot.h>
#endif
#if WITH_IMPLOT3D
#include <implot3d.h>
#endif
THIRD_PARTY_INCLUDES_END

#include <Fonts/IconsFontAwesome.h>
#include <TBImGuiUtils.h>
#include <TBImGuiDemo.h>

#include "TBImGuiExamplesMenuBar.h"

void UTBImGuiExamplesWindow_Base::Draw()
{
	if (bWantsFocus)
	{
		ImGui::SetNextWindowFocus();
		bWantsFocus = false;
	}
	
	ImGui::SetNextWindowSize(WindowSize, ImGuiCond_FirstUseEver);
	
	DrawWindow();
	
	if (!bWindowOpen)
	{
		UTBImGuiExamplesMenuBar* const MenuBar = GetTypedOuter<UTBImGuiExamplesMenuBar>();
		if (ensureAlwaysMsgf(IsValid(MenuBar), TEXT("Invalid menu bar?! Cannot remove window!")))
		{
			MenuBar->RequestCloseWindow(this);
		}
	}
}

void UTBImGuiExamplesWindow_Base::DrawWindow()
{
	if (ImGui::Begin(TCHAR_TO_ANSI(*WindowName), &bWindowOpen))
	{
		DrawWindowContent();
	}

	ImGui::End();
}

void UTBImGuiExamplesWindow_Base::FocusWindow()
{
	bWantsFocus = true;
}

void UTBImGuiExamplesWindow_Base::CloseWindow()
{
	bWindowOpen = false;
}

FString UTBImGuiExamplesWindow_Base::GetWindowName() const
{
	return WindowName;
}

FString UTBImGuiExamplesWindow_Base::GetMenuCategory() const
{
	return MenuCategory;
}

bool UTBImGuiExamplesWindow_Base::ShouldRegisterWindow() const
{
	return true;
}

#pragma region ImGui Example Windows

UTBImGuiExamplesWindow_ImGuiDemo::UTBImGuiExamplesWindow_ImGuiDemo()
{
	WindowName = TEXT("ImGui Demo");
	MenuCategory = TEXT("ImGui Examples");
}

void UTBImGuiExamplesWindow_ImGuiDemo::DrawWindow()
{
	ImGui::ShowDemoWindow(&bWindowOpen);
}

UTBImGuiExamplesWindow_ImPlotDemo::UTBImGuiExamplesWindow_ImPlotDemo()
{
	WindowName = TEXT("ImPlot Demo");
	MenuCategory = TEXT("ImGui Examples");
}

void UTBImGuiExamplesWindow_ImPlotDemo::DrawWindow()
{
#if WITH_IMPLOT
	ImPlot::ShowDemoWindow(&bWindowOpen);
#endif
}

bool UTBImGuiExamplesWindow_ImPlotDemo::ShouldRegisterWindow() const
{
#if WITH_IMPLOT
	return true;
#else 
	return false;
#endif
}

UTBImGuiExamplesWindow_ImPlot3DDemo::UTBImGuiExamplesWindow_ImPlot3DDemo()
{
	WindowName = TEXT("ImPlot 3D Demo");
	MenuCategory = TEXT("ImGui Examples");
}

void UTBImGuiExamplesWindow_ImPlot3DDemo::DrawWindow()
{
#if WITH_IMPLOT3D
	ImPlot3D::ShowDemoWindow(&bWindowOpen);
#endif
}

bool UTBImGuiExamplesWindow_ImPlot3DDemo::ShouldRegisterWindow() const
{
#if WITH_IMPLOT3D
	return true;
#else 
	return false;
#endif
}

UTBImGuiExamplesWindow_ImGuiDebugLog::UTBImGuiExamplesWindow_ImGuiDebugLog()
{
	WindowName = TEXT("ImGui Debug Log");
	MenuCategory = TEXT("ImGui Examples");
}

void UTBImGuiExamplesWindow_ImGuiDebugLog::DrawWindow()
{
	ImGui::ShowDebugLogWindow(&bWindowOpen);
}

UTBImGuiExamplesWindow_ImGuiMetrics::UTBImGuiExamplesWindow_ImGuiMetrics()
{
	WindowName = TEXT("ImGui Metrics");
	MenuCategory = TEXT("ImGui Examples");
}

void UTBImGuiExamplesWindow_ImGuiMetrics::DrawWindow()
{
	ImGui::ShowMetricsWindow(&bWindowOpen);
}

UTBImGuiExamplesWindow_ImGuiStyleEditor::UTBImGuiExamplesWindow_ImGuiStyleEditor()
{
	WindowName = TEXT("ImGui Style Editor");
	MenuCategory = TEXT("ImGui Examples");
}

void UTBImGuiExamplesWindow_ImGuiStyleEditor::DrawWindowContent()
{
	ImGui::ShowStyleEditor();
}

#pragma endregion 

UTBImGuiExamplesWindow_TBImGuiDemo::UTBImGuiExamplesWindow_TBImGuiDemo()
{
	WindowName = TEXT("Toolbox Demo");
	MenuCategory = TEXT("Unreal Examples");
}

void UTBImGuiExamplesWindow_TBImGuiDemo::DrawWindow()
{
	FTBImGuiDemo::ShowDemoWindow(&bWindowOpen);
}

#pragma region Actor List Example

UTBImGuiExamplesWindow_Actors::UTBImGuiExamplesWindow_Actors()
{
	WindowName = TEXT("Actor List");
	MenuCategory = TEXT("Unreal Examples");
}

void UTBImGuiExamplesWindow_Actors::OnWindowOpened()
{
	RefreshActorList();
}

void UTBImGuiExamplesWindow_Actors::DrawWindowContent()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Add menu button option to refresh the actor list manually.
			if (ImGui::Selectable("Refresh List"))
			{
				RefreshActorList();
			}
			
			ImGui::EndMenu();
		}
		
		// Search bar
		{
			const int32 LastSearchFilterLen = strlen(ActorSearchFilter);
			
			ImGui::SetNextItemSpan();
			const bool bTextCommited = ImGui::InputTextWithHint("##SearchBox", "Search...", ActorSearchFilter,
				IM_COUNTOF(ActorSearchFilter), ImGuiInputTextFlags_EnterReturnsTrue);

			// Reload the actor list if the search text changes or is commited with enter.
			if (bTextCommited || LastSearchFilterLen != strlen(ActorSearchFilter))
			{
				RefreshActorList();
			}
		}
		
		ImGui::EndMenuBar();
	}
	
	// If we have no actors, just show the following text.
	if (ActorList.IsEmpty())
	{
		ImGui::Text("No actors in world...");
		return;
	}
	
	// Create a tabular view of all actors in the ActorList in two collumns for the actor name and the actor class type.
	if (ImGui::BeginTable("##ActorListExampleTable", 2, ImGuiTableFlags_PadOuterX | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Sortable))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHide, 0.6f);
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch, 0.4f);
	
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();
		
		// Support sorting individual columns when the column header is clicked.
		if (ImGuiTableSortSpecs* Sorting = ImGui::TableGetSortSpecs())
		{
			if (Sorting->Specs && (Sorting->SpecsDirty || bSortActorList))
			{
				SortActorList(Sorting->Specs->ColumnIndex, Sorting->Specs->SortDirection == ImGuiSortDirection_Ascending);
				Sorting->SpecsDirty = false;
				bSortActorList = false;
			}
		}
		
		TArray<int32> RowsPendingRemoval;
		
		// Use ImGui list clipper to avoid having issues with rendering huge amounts of actors within this table.
		ImGuiListClipper Clipper;
		Clipper.Begin(ActorList.Num());
		while (Clipper.Step())
		{
			for (int32 RowIdx = Clipper.DisplayStart; RowIdx < Clipper.DisplayEnd; ++RowIdx)
			{
				ImGui::TableNextRowFirstColumn();
				ImGui::TableRowHighlightOnHovered();
	
				const TWeakObjectPtr<AActor>& ActorWeak = ActorList[RowIdx];
				AActor* const Actor = ActorWeak.Get();
				if (!IsValid(Actor))
				{
					ImGui::TextUnformatted("Invalid Entry! (Refresh list)");
					continue;
				}
				
				IMGUI_ID_SCOPE(RowIdx);
				
				// Show an eye icon button to toggle the actor hidden in game state and the actor name in the first column.
				{
					bool bNotHidden = !Actor->IsHidden();
					if (ImGui::CheckboxButton(ICON_FA_EYE, bNotHidden))
					{
						Actor->SetActorHiddenInGame(!bNotHidden);
					}
	
					ImGui::SameLine();
					ImGui::TextUnformatted(TCHAR_TO_ANSI(*Actor->GetName()));
				}
				
				// In the second column show the actors class type.
				if (ImGui::TableNextColumn())
				{
					const UClass* const ActorClass = Actor->GetClass();
					const FString ActorType = IsValid(ActorClass) ? ActorClass->GetName() : "Null Type";
					ImGui::TextUnformatted(TCHAR_TO_ANSI(*ActorType));
				}
				
				// Add a context menu so that when the row is right-clicked, we can destroy the actor.
				if (ImGui::TableRowBeginPopupContextItem("##OutlinerItemMenu"))
				{
					if (ImGui::Selectable("Destroy"))
					{
						RowsPendingRemoval.Add(RowIdx);
					}
				
					ImGui::EndPopup();
				}
			}
		}
		
		// If the actor(s) was destroyed through the context menu, remove them from the list.
		for (const int32 RowIdx : RowsPendingRemoval)
		{
			if (ActorList.IsValidIndex(RowIdx))
			{
				if (ActorList[RowIdx].IsValid())
				{
					ActorList[RowIdx]->Destroy();
				}
	
				ActorList.RemoveAt(RowIdx);
			}
		}
		
		ImGui::EndTable();
	}
}

void UTBImGuiExamplesWindow_Actors::RefreshActorList()
{
	ActorList.Empty();
	
	// Flag the actor list to be sorted after we have repopulated it with actors from the world.
	bSortActorList = true;
	
	const UWorld* const CurrentWorld = GetWorld();
	if (!IsValid(CurrentWorld))
	{
		return;
	}
	
	const FString SearchString = ActorSearchFilter ? ANSI_TO_TCHAR(ActorSearchFilter) : FString();
	const bool bSearchActive = !SearchString.IsEmpty();
	
	// Populate the ActorList with all actors in the world that are not abstract and match the search filter if it is active.
	for (TActorIterator<AActor> ActorItr(CurrentWorld); ActorItr; ++ActorItr)
	{
		AActor* const Actor = *ActorItr;
		if (!IsValid(Actor) || Actor->GetClass()->HasAnyClassFlags(CLASS_Abstract))
		{
			continue;
		}
		
		if (bSearchActive && !Actor->GetName().Contains(SearchString))
		{
			continue;
		}
		
		ActorList.Add(Actor);
	}
}

void UTBImGuiExamplesWindow_Actors::SortActorList(const int32 InColumn, bool bAscending)
{
	// Use Epics sorting algo to sort the actor list based on the given column and sort direction.
	ActorList.Sort([InColumn, bAscending](const TWeakObjectPtr<AActor>& First, const TWeakObjectPtr<AActor>& Second)
	{
		switch (InColumn)
		{
			// Sort by actor name
			case 0:	
				{
					const FString FirstName = GetNameSafe(First.Get());
					const FString SecondName = GetNameSafe(Second.Get());
					if (bAscending)
					{
						return FirstName < SecondName;
					}
	
					return FirstName > SecondName;
				}
				// Sort by actor type
				case 1: 
				{
					const FString FirstType = First.IsValid() ? First->GetClass()->GetName() : "Null Type";
					const FString SecondType = Second.IsValid() ? Second->GetClass()->GetName() : "Null Type";
					if (bAscending)
					{
						return FirstType < SecondType;
					}
	
					return FirstType > SecondType;
				}
				default: break;
			}
	
			return false;
		});
}

#pragma endregion 

#pragma region Texture Example

UTBImGuiExamplesWindow_Texture::UTBImGuiExamplesWindow_Texture()
{
	WindowName = TEXT("Texture");
	MenuCategory = TEXT("Unreal Examples");
}

void UTBImGuiExamplesWindow_Texture::OnWindowOpened()
{
	CreateTexture();
}

void UTBImGuiExamplesWindow_Texture::DrawWindowContent()
{
	// Show the test texture we created in this example. This will be a simple checkerboard pattern.
	const ImVec2 Size = ImGui::GetContentRegionAvail();
	ImGui::Image(ImTextureRef(TestTexture), Size);
}

void UTBImGuiExamplesWindow_Texture::CreateTexture()
{
	// Create texture.
	static constexpr int32 SizeX = 1024;
	static constexpr int32 SizeY = 1024;
	TestTexture = UTexture2D::CreateTransient(SizeX, SizeY);
	if (!ensureAlwaysMsgf(IsValid(TestTexture), TEXT("Texture could not be created!")))
	{
		return;
	}
	
	// Apply a checkerboard pattern to texture.
	{
		TestTexture->SRGB = true;

		FTexturePlatformData* const PlatformData = TestTexture->GetPlatformData();
		if (!ensureMsgf(PlatformData && PlatformData->Mips.Num() > 0, TEXT("Invalid texture platform data! Cannot update texture!")))
		{
			return;
		}
		
		FTexture2DMipMap& Mip = PlatformData->Mips[0];
		void* const Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
		FColor* Pixels = static_cast<FColor*>(Data);
		
		static const FColor ColorA = FColor::Black;
		static const FColor ColorB = FColor::White;
		static constexpr int32 TileSize = 128;

		for (int32 Y = 0; Y < SizeY; ++Y)
		{
			for (int32 X = 0; X < SizeX; ++X)
			{
				const bool bUseA = ((X / TileSize) + (Y / TileSize)) % 2 == 0;
				Pixels[Y * SizeY + X] = bUseA ? ColorA : ColorB;
			}
		}
		
		Mip.BulkData.Unlock();
		TestTexture->UpdateResource();
	}
	
	// Try to send through NetImGui if we are connected when the window is opened.
#if WITH_NETIMGUI
	FTBImGuiUtils::SendTextureToNetImGui(TestTexture);
#endif
}

#pragma endregion

#pragma region Unit Graph Example

UTBImGuiExamplesWindow_UnitGraph::UTBImGuiExamplesWindow_UnitGraph()
{
	WindowName = TEXT("Unit Graph");
	MenuCategory = TEXT("Unreal Examples");
}

bool UTBImGuiExamplesWindow_UnitGraph::ShouldRegisterWindow() const
{
#if WITH_IMPLOT
	return true;
#else 
	return false;
#endif
}

ImPlotPoint UnitGraph_PlotPointGetter(int PlotPointIndex, void* Data)
{
	const TTBImGuiRingBuffer<FVector2D>* Buffer = static_cast<TTBImGuiRingBuffer<FVector2D>*>(Data);
	if (!Buffer)
	{
		return ImPlotPoint();
	}
	
	// Apply offset to plot index to support oldest to newest value when the buffer is full.
	const int32 OffsetIndex = (Buffer->GetWriteOffset() + PlotPointIndex) % Buffer->Num();
	if (!Buffer->IsValidIndex(OffsetIndex))
	{
		return ImPlotPoint();
	}
	
	const FVector2D& Point = Buffer->GetElement(OffsetIndex);
	return ImPlotPoint(Point.X, Point.Y);
}

// Return an average value of across a given number of frames.
float UnitGraph_GetAverage(const TTBImGuiRingBuffer<FVector2D>& Buffer, int32 FrameCount)
{
	const int32 Count = FMath::Min(FrameCount, Buffer.Num());
	if (Count <= 0)
	{
		return 0.0f;
	}
			
	const int32 Stride = Buffer.Num();
	int32 Idx = (Buffer.GetWriteOffset() - 1 + Stride) % Stride;
			
	double Sum = 0.0;
	for (int32 i = 0; i < Count; ++i)
	{
		Sum += Buffer.GetElement(Idx).Y;
		Idx = (Idx - 1 + Stride) % Stride;
	}
	    	
	return static_cast<float>(Sum / Count);
};

void UTBImGuiExamplesWindow_UnitGraph::DrawWindowContent()
{
#if WITH_IMPLOT
	if (const UWorld* const CurrentWorld = GetValid(GetWorld()))
	{
	    // Push current samples before computing averages so the newest frame counts.
	    const float CurrentWorldTime = CurrentWorld->GetTimeSeconds();
	    const float FrameTimeMs = (FApp::GetCurrentTime() - FApp::GetLastTime()) * 1000.0f;
	    const float GameThreadTimeMs = FPlatformTime::ToMilliseconds(GGameThreadTime);
	    const float RenderThreadMs = FPlatformTime::ToMilliseconds(GRenderThreadTime);
	    const float RHITTimeMs = FPlatformTime::ToMilliseconds(GRHIThreadTime);
	    const float GPUFrameTimeMs = FPlatformTime::ToMilliseconds(RHIGetGPUFrameCycles(0));
		
	    FramePoints.WriteNewElementUninitialized() = FVector2D(CurrentWorldTime, FrameTimeMs);
	    GamePoints.WriteNewElementUninitialized() = FVector2D(CurrentWorldTime, GameThreadTimeMs);
	    DrawPoints.WriteNewElementUninitialized() = FVector2D(CurrentWorldTime, RenderThreadMs);
	    RHITPoints.WriteNewElementUninitialized() = FVector2D(CurrentWorldTime, RHITTimeMs);
	    GPUPoints.WriteNewElementUninitialized() = FVector2D(CurrentWorldTime, GPUFrameTimeMs);
		
		// Draw averages row above plot.
		if (ImGui::BeginTable("##AverageTimingsTable", 6, ImGuiTableFlags_PadOuterX | ImGuiTableFlags_SizingStretchSame))
		{
			ImGui::TableSetupColumn("FPS", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Frame", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Game", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Draw", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("RHIT", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("GPU", ImGuiTableColumnFlags_WidthStretch);
		
			constexpr int32 AverageWindow = 100;
			const float AverageFramesMs = UnitGraph_GetAverage(FramePoints, AverageWindow);
			
			ImGui::TableNextRowFirstColumn();
			ImGui::TextColored(ImGui::ToImVec4(FColor::White), "FPS %i", AverageFramesMs > 0.0f ?  FMath::FloorToInt32(1000.0f / AverageFramesMs) : 0);
			ImGui::TableNextColumn();
			ImGui::TextColored(ImGui::ToImVec4(FColor::Green), "Frame %.2f (ms)", AverageFramesMs);
			ImGui::TableNextColumn();
			ImGui::TextColored(ImGui::ToImVec4(FColor::Red), "Game %.2f (ms)", UnitGraph_GetAverage(GamePoints, AverageWindow));
			ImGui::TableNextColumn();
			ImGui::TextColored(ImGui::ToImVec4(FColor::Cyan), "Draw %.2f (ms)", UnitGraph_GetAverage(DrawPoints, AverageWindow));
			ImGui::TableNextColumn();
			ImGui::TextColored(ImGui::ToImVec4(FColor::Purple), "RHIT %.2f (ms)", UnitGraph_GetAverage(RHITPoints, AverageWindow));
			ImGui::TableNextColumn();
			ImGui::TextColored(ImGui::ToImVec4(FColor::Yellow), "GPU %.2f (ms)", UnitGraph_GetAverage(GPUPoints, AverageWindow));
			
			ImGui::EndTable();
		}
		
		// Plot timings for this frame.
	    if (ImPlot::BeginPlot("##UnitGraphPlot", ImVec2(-1, -1), ImPlotFlags_None))
	    {
	        ImPlot::SetupAxes("##Value", "Time (ms)", ImPlotAxisFlags_NoLabel, ImPlotAxisFlags_None);
	        ImPlot::SetupAxisLimits(ImAxis_X1, CurrentWorldTime - 10.0f, CurrentWorldTime, ImPlotCond_Always);
	        ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, 50.0f, ImPlotCond_Once);
			
	        ImPlotSpec FrameSpec; 
	    	FrameSpec.LineColor = ImGui::ToImVec4(FColor::Green);
	        ImPlot::PlotLineG("Frame", UnitGraph_PlotPointGetter, &FramePoints, FramePoints.Num(), FrameSpec);
			
	        ImPlotSpec GameSpec; 
	    	GameSpec.LineColor = ImGui::ToImVec4(FColor::Red);
	        ImPlot::PlotLineG("Game", UnitGraph_PlotPointGetter, &GamePoints, GamePoints.Num(), GameSpec);
			
	        ImPlotSpec DrawSpec; 
	    	DrawSpec.LineColor = ImGui::ToImVec4(FColor::Cyan);
	        ImPlot::PlotLineG("Draw", UnitGraph_PlotPointGetter, &DrawPoints, DrawPoints.Num(), DrawSpec);
			
	        ImPlotSpec RHITSpec; 
	    	RHITSpec.LineColor = ImGui::ToImVec4(FColor::Purple);
	        ImPlot::PlotLineG("RHIT", UnitGraph_PlotPointGetter, &RHITPoints, RHITPoints.Num(), RHITSpec);
			
	        ImPlotSpec GPUSpec; 
	    	GPUSpec.LineColor = ImGui::ToImVec4(FColor::Yellow);
	        ImPlot::PlotLineG("GPU", UnitGraph_PlotPointGetter, &GPUPoints, GPUPoints.Num(), GPUSpec);
			
	        ImPlot::EndPlot();
	    }
	}
#endif
}

#pragma endregion

#pragma region Player Trail Graph Example

UTBImGuiExamplesWindow_PlayerHistory::UTBImGuiExamplesWindow_PlayerHistory()
{
	WindowName = TEXT("Player History");
	MenuCategory = TEXT("Unreal Examples");
}

bool UTBImGuiExamplesWindow_PlayerHistory::ShouldRegisterWindow() const
{
#if WITH_IMPLOT3D
	return true;
#else 
	return false;
#endif
}

void UTBImGuiExamplesWindow_PlayerHistory::DrawWindowContent()
{
#if WITH_IMPLOT3D
	if (const UWorld* const CurrentWorld = GetValid(GetWorld()))
	{
		if (const APlayerController* const FirstPlayerController = GetValid(CurrentWorld->GetFirstPlayerController()))
		{
			if (const APawn* const Pawn = GetValid(FirstPlayerController->GetPawn()))
			{
				// Plot players position on the 3D graph.
				const FVector PlayerLocation = Pawn->GetActorLocation();
				static constexpr float PlayerUpdateTolerance = 1.0f; // Have to move least 1 unit to plot a new point on the graph.
				if (!PlayerLocation.Equals(LastPlayerLocation, PlayerUpdateTolerance))
				{
					PlayerXPoints.WriteNewElementUninitialized() = PlayerLocation.X;
					PlayerYPoints.WriteNewElementUninitialized() = PlayerLocation.Y;
					PlayerZPoints.WriteNewElementUninitialized() = PlayerLocation.Z;
					LastPlayerLocation = PlayerLocation;
				}
				
				checkf(PlayerXPoints.Num() == PlayerYPoints.Num() && PlayerYPoints.Num() == PlayerZPoints.Num(), 
					TEXT("Player offset point buffers are out of sync!"));
				
				// Draw 3D plot of players location history.
				if (ImPlot3D::BeginPlot("##PlayerOffsetPlot", ImVec2(-1, -1), ImPlot3DFlags_None))
				{
					ImPlot3D::SetupAxes("X", "Y", "Z", ImPlot3DAxisFlags_None, 
						ImPlot3DAxisFlags_None, ImPlot3DAxisFlags_None);
					ImPlot3D::SetupAxisLimits(ImAxis3D_X, -1500.0f, 1500.0f, ImPlot3DCond_Once);
					ImPlot3D::SetupAxisLimits(ImAxis3D_Y, -1500.0f, 1500.0f, ImPlot3DCond_Once);
					ImPlot3D::SetupAxisLimits(ImAxis3D_Z, -50.0f, 1500.0f, ImPlot3DCond_Once);
	
					// Draw player trail on a 3D graph.
					{
						ImPlot3DSpec PlotSpec;
						PlotSpec.LineColor = ImGui::ToImVec4(FColor::Red);
						PlotSpec.LineWeight = 2.0f;
						PlotSpec.Offset = PlayerXPoints.GetWriteOffset();
						ImPlot3D::PlotLine("Location History", PlayerXPoints.GetData(), PlayerYPoints.GetData(), 
							PlayerZPoints.GetData(), PlayerXPoints.Num(), PlotSpec);
					}
					
					// Draw wireframe box for the player.
					{
						FVector Origin;
						FVector Extents;
						Pawn->GetActorBounds(true, Origin, Extents);
						
						const FTransform Transform(Pawn->GetActorQuat());
						
						const FVector Vertices[8] = {
							Origin + Transform.TransformPosition(FVector(-Extents.X, -Extents.Y, -Extents.Z)),
							Origin + Transform.TransformPosition(FVector( Extents.X, -Extents.Y, -Extents.Z)),
							Origin + Transform.TransformPosition(FVector( Extents.X,  Extents.Y, -Extents.Z)),
							Origin + Transform.TransformPosition(FVector(-Extents.X,  Extents.Y, -Extents.Z)),
							Origin + Transform.TransformPosition(FVector(-Extents.X, -Extents.Y,  Extents.Z)),
							Origin + Transform.TransformPosition(FVector( Extents.X, -Extents.Y,  Extents.Z)),
							Origin + Transform.TransformPosition(FVector( Extents.X,  Extents.Y,  Extents.Z)),
							Origin + Transform.TransformPosition(FVector(-Extents.X,  Extents.Y,  Extents.Z))
						};

						static const int32 Edges[12][2] = {
							{0,1}, {1,2}, {2,3}, {3,0},
							{4,5}, {5,6}, {6,7}, {7,4},
							{0,4}, {1,5}, {2,6}, {3,7}
						};

						ImPlot3DSpec BoxSpec;
						BoxSpec.LineColor = ImGui::ToImVec4(FColor::Blue);
						BoxSpec.LineWeight = 1.5f;

						for (int32 EdgeIdx = 0; EdgeIdx < 12; ++EdgeIdx)
						{
							const int32 A = Edges[EdgeIdx][0];
							const int32 B = Edges[EdgeIdx][1];

							double X[2] = { Vertices[A].X, Vertices[B].X };
							double Y[2] = { Vertices[A].Y, Vertices[B].Y };
							double Z[2] = { Vertices[A].Z, Vertices[B].Z };
							
							ImPlot3D::PlotLine("Local Player", X, Y, Z, 2, BoxSpec);
						}
					}
	
					ImPlot3D::EndPlot();
				}
			}
		}
	}
#endif
}

#pragma endregion