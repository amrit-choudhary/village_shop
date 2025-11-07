#ifdef VG_WIN

/**
 * Main entry point for the Windows application.
 * This file is specific to Windows and uses the Win32 API for application management.
 */

#include <d3d12.h>
#include <dxgi1_6.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <wrl.h>

#include <atomic>
#include <cstdio>
#include <iostream>
#include <thread>

#include "../../main/main_win.h"

void AttachDebugConsole() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    std::cout.clear();
    std::cerr.clear();
}

using Microsoft::WRL::ComPtr;

LRESULT CALLBACK WindowProcW(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_NCCREATE: {
            CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            ME::GameMain* pGame = reinterpret_cast<ME::GameMain*>(pCreate->lpCreateParams);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pGame));
            break;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }

        case WM_SYSKEYUP:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_KEYDOWN: {
            ME::GameMain* pGame = reinterpret_cast<ME::GameMain*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            if (pGame) {
                pGame->HandleInput(msg, wParam, lParam);
            }
            return 0;
        }
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
#ifdef _DEBUG
    AttachDebugConsole();
#endif

    // Initialize the game
    char exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);
    ME::SetPaths(exePath, nullptr);

    ME::GameMain game;

    // Initialize the window class.
    WNDCLASSEXW windowClass = {0};
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProcW;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"VillageGame";
    RegisterClassExW(&windowClass);

    // Desired client area size
    int clientWidth = 1200;
    int clientHeight = 900;

    RECT rect = {0, 0, clientWidth, clientHeight};
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    HWND hWnd = CreateWindowExW(0, windowClass.lpszClassName, L"VillageGame", WS_OVERLAPPEDWINDOW, 60, 60, windowWidth,
                                windowHeight, nullptr, nullptr, hInstance, &game);

    game.Init(hWnd);
    ShowWindow(hWnd, nCmdShow);

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Update the game
        game.Update();
    }

    // Cleanup
    game.ShutDownGameSystems();

    FreeConsole();
    return 0;
}

#endif  // VG_WIN
