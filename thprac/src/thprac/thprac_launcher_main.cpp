#include "thprac_launcher_main.h"
#include "thprac_gui_locale.h"
#include "thprac_launcher_cfg.h"
#include "thprac_launcher_games.h"
#include "thprac_launcher_links.h"
#include "thprac_launcher_tools.h"
#include "thprac_launcher_utils.h"
#include "thprac_utils.h"
#include <ShlObj.h>
#include <Windows.h>
#include <Shlwapi.h>
#include <cstdio>
#include <sstream>
#include <d3d9.h>

namespace THPrac {
const char* gTabToOpen = nullptr;
LauncherTrigger gLauncherTrigger = LAUNCHER_NOTHING;
void GuiLauncherMainSwitchTab(const char* tab)
{
    gTabToOpen = tab;
}
bool GuiTabItem(const char* tabText)
{
    auto flag = 0;
    if (gTabToOpen && !strcmp(gTabToOpen, tabText)) {
        flag = ImGuiTabItemFlags_SetSelected;
        gTabToOpen = nullptr;
    }
    return ImGui::BeginTabItem(tabText, 0, flag);
}
void ErrorMsgBox(th_glossary_t textRef)
{
    auto title = utf8_to_utf16(XSTR(THPRAC_PR_ERROR));
    auto text = utf8_to_utf16(XSTR(textRef));
    MessageBoxW(NULL, text.c_str(), title.c_str(), MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
}

void GuiLauncherLocaleInit()
{
    if (LauncherCfgInit(true)) {
        if (!Gui::LocaleInitFromCfg()) {
            Gui::LocaleAutoSet();
        }
        LauncherCfgClose();
    }
}
void GuiLauncherMainTrigger(LauncherTrigger trigger)
{
    gLauncherTrigger = trigger;
}

namespace Gui {
    extern LRESULT ImplWin32WndProcHandlerW(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

static LPDIRECT3D9 g_pD3D = NULL;
static LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS g_d3dpp = {};
static HWND hwnd = NULL;

ImVec2 LauncherWndGetSize() {
    return ImGui::GetIO().DisplaySize;
}

int LauncherWndMsgBox(const wchar_t* title, const wchar_t* text, int flag)
{
    return MessageBoxW(hwnd, text, title, flag);
}

// From thcrap
// https://github.com/thpatch/thcrap/blob/33894bbac0def84d4f6d89aee7735716d1543b52/thcrap_configure/src/configure_search.cpp#L56

// Work around a bug in Windows 7 and later by sending
// BFFM_SETSELECTION a second time.
// https://connect.microsoft.com/VisualStudio/feedback/details/518103/bffm-setselection-does-not-work-with-shbrowseforfolder-on-windows-7
typedef struct {
    ITEMIDLIST* path;
    int attempts;
} initial_path_t;

int CALLBACK SetInitialBrowsePathProc(HWND hWnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
    initial_path_t* ip = (initial_path_t*)pData;
    if (ip) {
        switch (uMsg) {
        case BFFM_INITIALIZED:
            ip->attempts = 0;
            // fallthrough
        case BFFM_SELCHANGED:
            if (ip->attempts < 2) {
                SendMessageW(hWnd, BFFM_SETSELECTION, FALSE, (LPARAM)ip->path);
                ip->attempts++;
            }
        }
    }
    return 0;
}

template <typename T>
struct ComRAII {
    T* p;

    operator bool() const { return !!p; }
    operator T*() const { return p; }
    T** operator&() { return &p; }
    T* operator->() const { return p; }
    T& operator*() const { return *p; }

    ComRAII()
        : p(NULL)
    {
    }
    explicit ComRAII(T* p)
        : p(p)
    {
    }
    ComRAII(const ComRAII<T>& other) = delete;
    ComRAII<T>& operator=(const ComRAII<T>& other) = delete;
    ~ComRAII()
    {
        if (p) {
            p->Release();
            p = NULL;
        }
    }
};

static int SelectFolderVista(HWND owner, PIDLIST_ABSOLUTE initial_path, PIDLIST_ABSOLUTE& pidl, const wchar_t* window_title)
{
    // Those two functions are absent in XP, so we have to load them dynamically
    HMODULE shell32 = LoadLibraryW(L"Shell32.dll");
    if (!shell32)
        return -1;
    auto pSHCreateItemFromIDList = (HRESULT(WINAPI*)(PCIDLIST_ABSOLUTE, REFIID, void**))GetProcAddress(shell32, "SHCreateItemFromIDList");
    auto pSHGetIDListFromObject = (HRESULT(WINAPI*)(IUnknown*, PIDLIST_ABSOLUTE*))GetProcAddress(shell32, "SHGetIDListFromObject");
    if (!pSHCreateItemFromIDList || !pSHGetIDListFromObject)
        return -1;

    ComRAII<IFileDialog> pfd;
    if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
        return -1;
    if (!pfd)
        return -1;

    {
        ComRAII<IShellItem> psi;
        pSHCreateItemFromIDList(initial_path, IID_PPV_ARGS(&psi));
        if (!psi)
            return -1;
        pfd->SetDefaultFolder(psi);
    }

    pfd->SetOptions(
        FOS_NOCHANGEDIR | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM
        | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_DONTADDTORECENT);
    pfd->SetTitle(window_title);
    HRESULT hr = pfd->Show(owner);
    ComRAII<IShellItem> psi;
    if (SUCCEEDED(hr) && SUCCEEDED(pfd->GetResult(&psi))) {
        pSHGetIDListFromObject(psi, &pidl);
        return 0;
    }

    if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
        return 0;
    return -1;
}

static int SelectFolderXP(HWND owner, PIDLIST_ABSOLUTE initial_path, PIDLIST_ABSOLUTE& pidl, const wchar_t* window_title)
{
    BROWSEINFOW bi = { 0 };
    initial_path_t ip = { 0 };
    ip.path = initial_path;

    bi.lpszTitle = window_title;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NONEWFOLDERBUTTON | BIF_USENEWUI;
    bi.hwndOwner = owner;
    bi.lpfn = SetInitialBrowsePathProc;
    bi.lParam = (LPARAM)&ip;
    pidl = SHBrowseForFolderW(&bi);
    return 0;
}

std::wstring LauncherWndFolderSelect(const wchar_t* title)
{
    if (FAILED(CoInitialize(NULL)))
        return L"";
    defer(CoUninitialize());

    PIDLIST_ABSOLUTE initial_path = NULL;
    wchar_t path[MAX_PATH] = {};

    GetCurrentDirectoryW(MAX_PATH, path);
    SHParseDisplayName(path, NULL, &initial_path, 0, NULL);

    PIDLIST_ABSOLUTE pidl = NULL;
    if (-1 == SelectFolderVista(hwnd, initial_path, pidl, title)) {
        SelectFolderXP(hwnd, initial_path, pidl, title);
    }

    if (pidl) {
        defer(CoTaskMemFree(pidl));
        if (SHGetPathFromIDListW(pidl, path)) {
            return path;
        }
    }
    return L"";
}

std::wstring LauncherWndFileSelect(const wchar_t* title, const wchar_t* filter)
{
    OPENFILENAME ofn = {};
    wchar_t szFile[MAX_PATH] = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NODEREFERENCELINKS;
    GetOpenFileName(&ofn);
    return std::wstring(szFile);
}

void D3DResetDevice()
{
    Gui::ImplDX9InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    Gui::ImplDX9CreateDeviceObjects();
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (Gui::ImplWin32WndProcHandlerW(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            D3DResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

int GuiLauncherMain()
{
    int localeSwitch = -1;

    auto wndTitle = utf8_to_utf16(XSTR(THPRAC_LAUNCHER));

    HANDLE mutex = CreateMutex(NULL, TRUE, L"thprac launcher mutex");
    if (mutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS) {
        for (int i = 0; i < 3; ++i) {
            HWND existingApp = FindWindowW(0, wndTitle.c_str());
            if (existingApp) {
                ShowWindow(existingApp, SW_RESTORE);
                SetForegroundWindow(existingApp);
            }
        }
        return 0;
    }
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"thprac launcher window", NULL };
    ::RegisterClassEx(&wc);
    hwnd = ::CreateWindowExW(0, wc.lpszClassName, wndTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 960, 720, NULL, NULL, wc.hInstance, NULL);

    auto CreateDeviceD3D = [](HWND hWnd) -> bool {
        auto d3d9_dll = LoadLibraryW(L"d3d9.dll");
        if (!d3d9_dll)
            return false;
        auto _Direct3DCreate9 = GET_PROC_ADDRESS(d3d9_dll, Direct3DCreate9);
        if (!_Direct3DCreate9 || ((g_pD3D = _Direct3DCreate9(D3D_SDK_VERSION)) == NULL))
            return false;

        // Create the D3DDevice
        ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
        g_d3dpp.Windowed = TRUE;
        g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
        g_d3dpp.EnableAutoDepthStencil = TRUE;
        g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Present with vsync
        // g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
        if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
            return false;

        return true;
    };
    if (!CreateDeviceD3D(hwnd))
        return 0;

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    ImGui::CreateContext();

    if (!LauncherCfgInit()) {
        ErrorMsgBox(THPRAC_PR_ERR_LAUNCHER_CFG);
        return -1;
    }

    int theme;
    if (LauncherSettingGet("theme", theme)) {
        // LauncherSettingSet doesn't take int, only int&.
        // Passing 0 will call the overload with const char*
        int Sus = 0;
        const char* theme_user = NULL;
        // LauncherSettingGet only accepts signed ints but I want to do an unsigned comparison
        if ((unsigned int)theme > 2) {
            if (LauncherSettingGet("theme_user", theme_user) && theme_user) {
                std::wstring theme_path = LauncherGetDataDir() + L"themes\\" + utf8_to_utf16(theme_user);
                if (!PathFileExistsW(theme_path.c_str())) {
                    LauncherSettingSet("theme", Sus);
                    theme = Sus;
                }
            } else {
                LauncherSettingSet("theme", Sus);
                theme = Sus;
            }
        }

        if (theme_user) {
            SetTheme(theme, utf8_to_utf16(theme_user).c_str());
        } else {
            SetTheme(theme);
        }
    }

    Gui::ImplWin32Init(hwnd);
    Gui::ImplDX9Init(g_pd3dDevice);
    
    auto& io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.LogFilename = NULL;

    Gui::LocaleCreateMergeFont(Gui::LocaleGet(), 20.0f); // 30.0f LOCALE_ZH_CN LOCALE_EN_US LOCALE_JA_JP

    LauncherPeekUpd();

    auto MsgCheck = []() -> bool {
        MSG msg;
        bool ret = true;
        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            if (msg.message == WM_QUIT)
                ret = false;
        }
        return ret;
    };

    ImGui::GetStyle().DisplayWindowPadding = { 0, 0 };

    while (MsgCheck()) {
        Gui::ImplDX9NewFrame();
        Gui::ImplWin32NewFrame();
        RECT cr;
        GetClientRect(hwnd, &cr);
        io.DisplaySize = ImVec2(cr.right - cr.left, cr.bottom - cr.top);
        ImGui::NewFrame();

        std::string wndTitleText = XSTR(THPRAC_LAUNCHER);
        if (LauncherIsChkingUpd()) {
            wndTitleText += " // Checking for updates...";
        }
        wndTitleText = LauncherIsChkingUpd() ? XSTR(THPRAC_LAUNCHER_CHECKING_UPDATE) : XSTR(THPRAC_LAUNCHER);
        SetWindowTextW(hwnd, utf8_to_utf16(wndTitleText.c_str()).c_str());

        ImGui::Begin(wndTitleText.c_str(), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowPos({ 0.0f, 0.0f });
        ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);

        if (ImGui::BeginTabBar("MenuTabBar")) {
            if (GuiTabItem(XSTR(THPRAC_GAMES))) {
                ImGui::BeginChild("##games");
                LauncherGamesGuiUpd();
                ImGui::EndChild();
                ImGui::EndTabItem();
            }
            if (GuiTabItem(XSTR(THPRAC_LINKS))) {
                ImGui::BeginChild("##links");
                LauncherLinksGuiUpd();
                ImGui::EndChild();
                ImGui::EndTabItem();
            }
            if (GuiTabItem(XSTR(THPRAC_TOOLS))) {
                ImGui::BeginChild("##tools");
                LauncherToolsGuiUpd();
                ImGui::EndChild();
                ImGui::EndTabItem();
            }
            if (GuiTabItem(XSTR(THPRAC_SETTINGS))) {
                ImGui::BeginChild("##settings");
                LauncherCfgGuiUpd();
                ImGui::EndChild();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        LauncherChkUpdPopup();

        ImGui::End();

        ImGui::EndFrame();
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, {}, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0) {
            ImGui::Render();
            Gui::ImplDX9RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            D3DResetDevice();
    }

    CloseHandle(mutex);

    return 0;
}

}
