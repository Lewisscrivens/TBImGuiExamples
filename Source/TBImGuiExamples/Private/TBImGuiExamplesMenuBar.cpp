// Copyright Lewis Scrivens. All Rights Reserved.

#include "TBImGuiExamplesMenuBar.h"

THIRD_PARTY_INCLUDES_START
#include <imgui.h>
THIRD_PARTY_INCLUDES_END

void UTBImGuiExamplesMenuBar::Initialize()
{
	// Use Epic's UObject reflection to populate menu bar with windows.
	TArray<UClass*> WindowClasses;
	GetDerivedClasses(UTBImGuiExamplesWindow_Base::StaticClass(), WindowClasses, true);
	for (UClass* const WindowClass : WindowClasses)
	{
		if (!IsValid(WindowClass) || WindowClass->HasAnyClassFlags(CLASS_Abstract))
		{
			continue;
		}
		
		UTBImGuiExamplesWindow_Base* const WindowCDO = WindowClass->GetDefaultObject<UTBImGuiExamplesWindow_Base>();
		if (!IsValid(WindowCDO) || !WindowCDO->ShouldRegisterWindow())
		{
			continue;
		}
		
		FTbImGuiExamplesMenuBarCategory& MenuBarCategory = MenuBar.FindOrAdd(WindowCDO->GetMenuCategory());
		MenuBarCategory.RegisteredWindows.AddUnique(WindowClass);
	}
}

void UTBImGuiExamplesMenuBar::DrawMenuBar()
{
	// Create a menu bar with options to open various ImGui example windows.
	if (ImGui::BeginMainMenuBar())
	{
		for (const TPair<FString, FTbImGuiExamplesMenuBarCategory>& MenuBarEntry : MenuBar)
		{
			const FString& CategoryName = MenuBarEntry.Key;
			const FTbImGuiExamplesMenuBarCategory& Category = MenuBarEntry.Value;
			if (!Category.RegisteredWindows.IsEmpty())
			{
				if (ImGui::BeginMenu(TCHAR_TO_ANSI(*CategoryName)))
				{
					for (const TSoftClassPtr<UTBImGuiExamplesWindow_Base>& WindowClass : Category.RegisteredWindows)
					{
						UTBImGuiExamplesWindow_Base* const WindowCDO = WindowClass->GetDefaultObject<UTBImGuiExamplesWindow_Base>();
						if (!IsValid(WindowCDO))
						{
							continue;
						}
						
						if (ImGui::MenuItem(TCHAR_TO_ANSI(*WindowCDO->GetWindowName())))
						{
							// If we try to open a window that is already open, just focus it.
							{
								TObjectPtr<UTBImGuiExamplesWindow_Base>* FoundWindow = OpenWindows.FindByPredicate([WindowClass] 
									(const TObjectPtr<UTBImGuiExamplesWindow_Base>& Window)
								{
									return Window.GetClass() == WindowClass.Get();
								});
							
								if (FoundWindow && IsValid(*FoundWindow))
								{
									FoundWindow->Get()->FocusWindow();
									continue;
								}
							}
							
							// Otherwise create a new window and add it to the OpenWindows list 
							// which is drawn in DrawWindows.
							UTBImGuiExamplesWindow_Base* const NewWindow = NewObject<UTBImGuiExamplesWindow_Base>(this, WindowClass.Get());
							check(IsValid(NewWindow));
							OpenWindows.Add(NewWindow);
							NewWindow->OnWindowOpened();
						}
					}
					
					ImGui::EndMenu();
				}
			}
		}
		
		ImGui::EndMainMenuBar();
	}
}

void UTBImGuiExamplesMenuBar::DrawWindows()
{
	// Process window removals before drawing the latest frame.
	{
		for (UTBImGuiExamplesWindow_Base* const Window : CloseWindows)
		{
			if (!IsValid(Window))
			{
				continue;
			}
		
			OpenWindows.Remove(Window);
		}
		
		CloseWindows.Empty();
	}
	
	// Draw all windows that are actively open.
	for (UTBImGuiExamplesWindow_Base* const Window : OpenWindows)
	{
		if (IsValid(Window))
		{
			Window->Draw();
		}
	}
}

void UTBImGuiExamplesMenuBar::RequestCloseWindow(UTBImGuiExamplesWindow_Base* InWindow)
{
	// Request close for next ImGui frame.
	if (IsValid(InWindow) && OpenWindows.Contains(InWindow))
	{
		CloseWindows.Add(InWindow);
	}
}