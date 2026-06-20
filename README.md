<div align="center">

# TBImGuiExamples

![UE Version](https://img.shields.io/badge/UE-5.7-blue)
![GitHub issues](https://img.shields.io/github/issues/Lewisscrivens/TBImGuiExamples)
![GitHub discussions](https://img.shields.io/github/discussions/Lewisscrivens/TBImGuiExamples)

</div>
<br>

An example project showcasing runtime and editor usage of the [TBImGui](https://github.com/Lewisscrivens/TBImGui) plugin.

<p>&nbsp;</p>

## 📄 Table of Contents

- [Preview](#-preview)
- [Requirements](#-requirements)
- [Runtime Examples](#-runtime-examples)
- [Editor Examples](#-editor-examples)
- [Getting Started](#-getting-started)
    - [Runtime](#Runtime)
    - [Editor](#Editor)
- [Project Configuration](#-project-configuration)
- [Commands](#-commands)
- [TBImGui Plugin](#-tbimgui-plugin)

<p>&nbsp;</p>

## 🖼️ Preview

<img width="3834" height="2111" alt="Preview-compressed" src="https://github.com/user-attachments/assets/02859377-9cdb-4557-9c6c-40443f44c61c" />

<p>&nbsp;</p>

## 💻 Requirements

- Unreal Engine **5.7**.
- Windows or Linux desktop platforms.
- The bundled [TBImGui](https://github.com/Lewisscrivens/TBImGui) plugin.

<p>&nbsp;</p>

## 🎮 Runtime Examples

At runtime the project creates a `UTBImGuiExamplesSubsystem` when the TBImGui runtime context is available.

The subsystem binds into the active ImGui context and draws example content every frame. An ImGui menu bar implemented
in the `UTBImGuiExamplesMenuBar` class is used to register and manage example windows using Unreal Engine's reflection system.

- `UTBImGuiExamplesSubsystem` — Handles per game-instance registry of example content using the `ExampleMenuBar`.
- `UTBImGuiExamplesMenuBar` — Contains example logic for a simple runtime ImGui menu bar and window management system.
- **Input** — Example input handling for showing/hiding ImGui content while returning control to the game with full NetImgui support.
- **Docking** — Example docking space in the games viewport.
- **ImPlot Examples** — Runtime graph examples using live engine timing data.
- **ImPlot3D Examples** — 3D plotting example showing player movement history in 3D space.
- **NetImgui Handling** — Visibility bindings for active connections through NetImgui.

| Example Window                              | Description                                                   |
|---------------------------------------------|---------------------------------------------------------------|
| **UTBImGuiExamplesWindow_Actors**           | Unreal editor world details panel within an ImGui window.     |
| **UTBImGuiExamplesWindow_Texture**          | Unreal engine custom texture rendered within an ImGui window. |
| **UTBImGuiExamplesWindow_UnitGraph**        | Unreal engines `Unit Graph` command in ImGui via ImPlot.      |
| **UTBImGuiExamplesWindow_PlayerHistory**    | ImPlot3D example of live 3D trail of player location history. |
| **UTBImGuiExamplesWindow_TBImGuiDemo**      | Custom TBImGui demo window showcasing Unreal binding.         |
| **UTBImGuiExamplesWindow_ImGuiDemo**        | Demo window for ImGui `ImGui::ShowDemoWindow()`               |
| **UTBImGuiExamplesWindow_ImPlotDemo**       | Demo window for ImPlot `ImPlot::ShowDemoWindow()`             |
| **UTBImGuiExamplesWindow_ImPlot3DDemo**     | Demo window for ImPlot3D `ImPlot3D::ShowDemoWindow()`         |
| **UTBImGuiExamplesWindow_ImGuiDebugLog**    | Dear ImGui's default debug log window.                        |
| **UTBImGuiExamplesWindow_ImGuiMetrics**     | Dear ImGui's default metrics window.                          |
| **UTBImGuiExamplesWindow_ImGuiStyleEditor** | Dear ImGui's default style menu.                              |

<p>&nbsp;</p>

## 🛠️ Editor Examples

Editor examples are registered by the `TBImGuiExamplesEditor` module when the TBImGui editor context is enabled.

These examples can be accessed from the editor's `Tools > ImGui > ImGui Examples` submenu.

- **Slate Window** — ImGui context and content rendered within a custom `SWindow`.
- **Slate Tabs** — ImGui context and content rendered within a 'SDockTab'.
- **Editor Main Frame** — ImGui window rendered in the editor main frame.

<img width="263" height="137" alt="UnrealEditor_Sdvvwj14os" src="https://github.com/user-attachments/assets/078291f3-3076-4d95-9746-7ce663a14e9c" />

<p>&nbsp;</p>

## 🚀 Getting Started

To get started, clone and open the project in Unreal Engine.

```bash
git clone https://github.com/Lewisscrivens/TBImGuiExamples.git
```

### Runtime

Press play in editor and use the `F1` key to toggle the ImGui content. 
If playing on a gamepad, you can use the `Special Left` button. 
There is also a command to toggle ImGui content `TB.ImGuiExamples.ToggleImGuiContent`.

### Editor 

Under the `Tools` menu, open the `ImGui Examples` submenu to access all the editor examples.

<p>&nbsp;</p>

## ⚙️ Project Configuration

### TBImGuiExamples Settings

The example project also includes its own project settings.

These can be found under:

```text
Project Settings > Toolbox > ImGui Toolbox Examples
```

| Setting                         | Description                                                                 |
|---------------------------------|-----------------------------------------------------------------------------|
| **bShowImGuiExamplesOnStartup** | Should the ImGui example menu/content be shown automatically on game start? |
| **InputModeEnabled**            | Input mode used by the ImGui context when example content is visible.       |
| **InputModeDisabled**           | Input mode used by the ImGui context when example content is hidden.        |

<p>&nbsp;</p>

## 📟 Commands

| Command                              | Description                                                                     |
|--------------------------------------|---------------------------------------------------------------------------------|
| `TB.ImGuiExamples.ToggleImGuiContent` | Toggle the ImGui visibility and input state for the runtime examples content.   |

The example project also supports the plugin commands documented in the main TBImGui README.

<p>&nbsp;</p>

## 📦 TBImGui Plugin

This project is built around the **TBImGui** plugin.

If you are looking for plugin instructions or documentation; see [TBImGui](https://github.com/Lewisscrivens/TBImGui).
