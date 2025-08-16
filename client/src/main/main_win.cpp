#ifdef VG_WIN

#include <d3d12.h>
#include <dxgi1_6.h>
#include <windows.h>
#include <wrl.h>

#include <atomic>
#include <cstdio>
#include <iostream>
#include <thread>

#include "../game/game.h"
#include "../game/game_breakout.h"
#include "../game/village_game.h"
#include "../input/input_manager.h"
#include "../misc/global_vars.h"
#include "../net/connection.h"
#include "src/file_io/ini/ini_parser.h"
#include "src/logging.h"
#include "src/misc/utils.h"
#include "src/physics/physics_system.h"
#include "src/time/time_manager.h"

using Microsoft::WRL::ComPtr;

LRESULT CALLBACK WindowProcW(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    // Initialize the window class.
    WNDCLASSEXW windowClass = {0};
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProcW;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"VillageGame";
    RegisterClassExW(&windowClass);

    // Create the window and store a handle to it.
    HWND hwnd = CreateWindowExW(0, windowClass.lpszClassName, L"VillageGame", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hwnd, nCmdShow);

    // 3. Initialize DXGI factory
    ComPtr<IDXGIFactory4> factory;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
        MessageBoxW(hwnd, L"Failed to create DXGI Factory", L"Error", MB_OK);
        return -1;
    }

    // 4. Create D3D12 device
    ComPtr<ID3D12Device> device;
    if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)))) {
        MessageBoxW(hwnd, L"Failed to create D3D12 Device", L"Error", MB_OK);
        return -1;
    }

    // 5. Create swap chain (minimal, no command queue yet)
    DXGI_SWAP_CHAIN_DESC1 scDesc = {};
    scDesc.BufferCount = 2;
    scDesc.Width = 800;
    scDesc.Height = 600;
    scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scDesc.SampleDesc.Count = 1;

    // Dummy command queue (required for swap chain)
    D3D12_COMMAND_QUEUE_DESC cqDesc = {};
    ComPtr<ID3D12CommandQueue> cmdQueue;
    device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&cmdQueue));

    ComPtr<IDXGISwapChain1> swapChain;
    factory->CreateSwapChainForHwnd(cmdQueue.Get(), hwnd, &scDesc, nullptr, nullptr, &swapChain);

    // 6. Message loop
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // No rendering yet
    }

    return 0;
}

#endif  // VG_WIN
