#include <d3d11.h>
#include "utils.h"
#include "../variables/variables.h"
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_win32.h"
#include "../include/imgui/imgui_impl_dx11.h"
#include "../include/imgui/imgui_internal.h"
#include "../include/imgui/imgui_stdlib.h"
#include "../save/Save.hpp"

#include <unordered_map>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace funcs {
    namespace d3d11 {
        bool CreateDeviceD3D(HWND hWnd)
        {
            DXGI_SWAP_CHAIN_DESC sd;
            ZeroMemory(&sd, sizeof(sd));
            sd.BufferCount = 2;
            sd.BufferDesc.Width = 0;
            sd.BufferDesc.Height = 0;
            sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.BufferDesc.RefreshRate.Numerator = 60;
            sd.BufferDesc.RefreshRate.Denominator = 1;
            sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.OutputWindow = hWnd;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.Windowed = TRUE;
            sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

            UINT createDeviceFlags = 0;
            D3D_FEATURE_LEVEL featureLevel;
            const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
            HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &vars::d3d11::g_pSwapChain, &vars::d3d11::g_pd3dDevice, &featureLevel, &vars::d3d11::g_pd3dDeviceContext);
            if (res == DXGI_ERROR_UNSUPPORTED)
                res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &vars::d3d11::g_pSwapChain, &vars::d3d11::g_pd3dDevice, &featureLevel, &vars::d3d11::g_pd3dDeviceContext);
            if (res != S_OK)
                return false;

            CreateRenderTarget();
            return true;
        }

        void CleanupDeviceD3D()
        {
            CleanupRenderTarget();
            if (vars::d3d11::g_pSwapChain) { vars::d3d11::g_pSwapChain->Release(); vars::d3d11::g_pSwapChain = nullptr; }
            if (vars::d3d11::g_pd3dDeviceContext) { vars::d3d11::g_pd3dDeviceContext->Release(); vars::d3d11::g_pd3dDeviceContext = nullptr; }
            if (vars::d3d11::g_pd3dDevice) { vars::d3d11::g_pd3dDevice->Release(); vars::d3d11::g_pd3dDevice = nullptr; }
        }

        void CreateRenderTarget()
        {
            ID3D11Texture2D* pBackBuffer;
            vars::d3d11::g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
            vars::d3d11::g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &vars::d3d11::g_mainRenderTargetView);
            pBackBuffer->Release();
        }

        void CleanupRenderTarget()
        {
            if (vars::d3d11::g_mainRenderTargetView) { vars::d3d11::g_mainRenderTargetView->Release(); vars::d3d11::g_mainRenderTargetView = nullptr; }
        }

        LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_LBUTTONDOWN) {
                if (GetMessageExtraInfo() == 4919) {
                    return 0;
                }
            }
            if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
                return true;

            switch (msg)
            {
            case WM_SIZE:
                if (wParam == SIZE_MINIMIZED)
                    return 0;
                vars::d3d11::g_ResizeWidth = (UINT)LOWORD(lParam);
                vars::d3d11::g_ResizeHeight = (UINT)HIWORD(lParam);
                return 0;
                break;
            case WM_CLOSE:
                Data::Write();
                vars::Thread::g_ThreadShouldExit = true;
                WaitForSingleObject(vars::Thread::hThread, INFINITE);
                CloseHandle(vars::Thread::hThread);
                PostQuitMessage(0);
            case WM_SYSCOMMAND:
                if (wParam == SC_CLOSE) {
                    Data::Write();
                    vars::Thread::g_ThreadShouldExit = true;
                    WaitForSingleObject(vars::Thread::hThread, INFINITE);
                    CloseHandle(vars::Thread::hThread);
                    PostQuitMessage(0);
                    break;
                }
            }
            return ::DefWindowProcW(hWnd, msg, wParam, lParam);
        }
    }
    namespace usefull {
        float ImLerp(float current, float target, float speed) {
            float deltaTime = ImGui::GetIO().DeltaTime;
            return current + (target - current) * speed * deltaTime;
        }

        bool openLink(std::string link) {
            if (ShellExecuteA(nullptr, "open", link.c_str(), nullptr, nullptr, SW_SHOWNORMAL)) {
                return true;
            }
            return false;
        }
        std::string KeyCodeToString(int keyCode) {
            static const std::unordered_map<int, std::string> keyMap = {
                {0x41, "A"}, {0x42, "B"}, {0x43, "C"}, {0x44, "D"}, {0x45, "E"},
                {0x46, "F"}, {0x47, "G"}, {0x48, "H"}, {0x49, "I"}, {0x4A, "J"},
                {0x4B, "K"}, {0x4C, "L"}, {0x4D, "M"}, {0x4E, "N"}, {0x4F, "O"},
                {0x50, "P"}, {0x51, "Q"}, {0x52, "R"}, {0x53, "S"}, {0x54, "T"},
                {0x55, "U"}, {0x56, "V"}, {0x57, "W"}, {0x58, "X"}, {0x59, "Y"},
                {0x5A, "Z"},

                {0x30, "0"}, {0x31, "1"}, {0x32, "2"}, {0x33, "3"}, {0x34, "4"},
                {0x35, "5"}, {0x36, "6"}, {0x37, "7"}, {0x38, "8"}, {0x39, "9"},

                {VK_LBUTTON, "LMOUSE BUTTON"},
                {VK_RBUTTON, "RMOUSE BUTTON"},
                {VK_MBUTTON, "MMOUSE BUTTON"},
                {VK_XBUTTON1, "MOUSE4"},
                {VK_XBUTTON2, "MOUSE5"},

                {VK_SHIFT, "SHIFT"},
                {VK_LSHIFT, "LSHIFT"},
                {VK_RSHIFT, "RSHIFT"},
                {VK_CONTROL, "CTRL"},
                {VK_LCONTROL, "LCTRL"},
                {VK_RCONTROL, "RCTRL"},
                {VK_MENU, "ALT"},
                {VK_LMENU, "LALT"},
                {VK_RMENU, "RALT"},

                {VK_SPACE, "SPACE"},
                {VK_RETURN, "ENTER"},
                {VK_ESCAPE, "ESCAPE"},
                {VK_BACK, "BACKSPACE"},
                {VK_TAB, "TAB"},
                {VK_CAPITAL, "CAPSLOCK"},
                {VK_DELETE, "DELETE"},
                {VK_INSERT, "INSERT"},
                {VK_HOME, "HOME"},
                {VK_END, "END"},
                {VK_PRIOR, "PAGEUP"},
                {VK_NEXT, "PAGEDOWN"},

                {VK_LEFT, "LEFT"},
                {VK_RIGHT, "RIGHT"},
                {VK_UP, "UP"},
                {VK_DOWN, "DOWN"},

                {VK_F1, "F1"}, {VK_F2, "F2"}, {VK_F3, "F3"}, {VK_F4, "F4"},
                {VK_F5, "F5"}, {VK_F6, "F6"}, {VK_F7, "F7"}, {VK_F8, "F8"},
                {VK_F9, "F9"}, {VK_F10, "F10"}, {VK_F11, "F11"}, {VK_F12, "F12"},

                {VK_NUMPAD0, "NUMPAD0"}, {VK_NUMPAD1, "NUMPAD1"},
                {VK_NUMPAD2, "NUMPAD2"}, {VK_NUMPAD3, "NUMPAD3"},
                {VK_NUMPAD4, "NUMPAD4"}, {VK_NUMPAD5, "NUMPAD5"},
                {VK_NUMPAD6, "NUMPAD6"}, {VK_NUMPAD7, "NUMPAD7"},
                {VK_NUMPAD8, "NUMPAD8"}, {VK_NUMPAD9, "NUMPAD9"},
                {VK_DIVIDE, "NUMPAD /"}, {VK_MULTIPLY, "NUMPAD *"},
                {VK_SUBTRACT, "NUMPAD -"}, {VK_ADD, "NUMPAD +"},
                {VK_DECIMAL, "NUMPAD ."},

                {VK_NUMLOCK, "NUMLOCK"},
                {VK_SCROLL, "SCROLLLOCK"},
                {VK_PAUSE, "PAUSE"},
            };

            auto it = keyMap.find(keyCode);
            if (it != keyMap.end())
                return it->second;

            return "UNKNOWN";
        }
        bool IsMouseHoveringRect(ImVec2 rMin, ImVec2 rMax) {
            POINT mouseP;
            GetCursorPos(&mouseP);

            if ((float)mouseP.x >= rMin.x && (float)mouseP.y >= rMin.y
                && (float)mouseP.x <= rMax.x && (float)mouseP.y <= rMax.y) {
                return true;
            }
            return false;
        }
    }
}
