#pragma once
#include <imgui.h>
#include <string>

namespace THPrac {
enum LauncherTrigger {
    LAUNCHER_NOTHING = 0,
	LAUNCHER_RESTART,
	LAUNCHER_CLOSE,
    LAUNCHER_MINIMIZE,
    LAUNCHER_RESET,
};

int GuiLauncherMain();
void GuiLauncherMainSwitchTab(const char* tab);
void GuiLauncherMainTrigger(LauncherTrigger trigger);
void GuiLauncherLocaleInit();

int LauncherWndMsgBox(const wchar_t* title, const wchar_t* text, int flag);
ImVec2 LauncherWndGetSize();
std::wstring LauncherWndFolderSelect(const wchar_t* title = nullptr);
std::wstring LauncherWndFileSelect(const wchar_t* title = nullptr, const wchar_t* filter = nullptr);
}
