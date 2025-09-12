#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define U8T(text) reinterpret_cast<const char*>(u8##text)
#define col255to01(r, g, b, a) (ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f))

#include <iostream>
#include "include/imgui/imgui.h"
#include "include/imgui/imgui_impl_dx11.h"
#include "include/imgui/imgui_impl_win32.h"
#include "include/imgui/imgui_internal.h"
#include "include/imgui/imgui_stdlib.h"
#include <Windows.h>
#include "resource/resource.h"
#include <condition_variable>
#include "font/font.h"
#include "images/images.hpp"
#include "variables/variables.h"
#include "save/Save.hpp"
#include "functions/Functions.h"
#include "ui/ui.h"
#include "MainThread.hpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Data::Load();
    vars::Thread::hThread = CreateThread(nullptr, 0, MainThread, nullptr, 0, nullptr);
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, funcs::d3d11::WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Pustote AutoClicker v1.2", nullptr };
    wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
    ::RegisterClassExW(&wc);
    vars::misc::window::hwnd = ::CreateWindowW(wc.lpszClassName, L"Pustote AutoClicker v1.2", WS_POPUP & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, GetSystemMetrics(SM_CXSCREEN) / 2.0f - 275.0f, GetSystemMetrics(SM_CYSCREEN) / 2.0f - 127.5f, 550, 256, nullptr, nullptr, wc.hInstance, nullptr);
    HRGN hRgn = CreateRoundRectRgn(0, 0, 550, 255, 9 * 2, 9 * 2);
    SetWindowRgn(vars::misc::window::hwnd, hRgn, TRUE);
    GetWindowRect(vars::misc::window::hwnd, &vars::misc::window::windowRect);
    float radius = 9.0f;
    if (!funcs::d3d11::CreateDeviceD3D(vars::misc::window::hwnd))
    {
        funcs::d3d11::CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    ::ShowWindow(vars::misc::window::hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(vars::misc::window::hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImFont* fontCyr = io.Fonts->AddFontFromMemoryCompressedTTF(myfont, myfont_size, 14.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

    io.FontDefault = fontCyr;

    ImGuiStyle& st = ImGui::GetStyle();
    st.WindowBorderSize = 0.0f;
    st.Colors[ImGuiCol_Border] = ImVec4(col255to01(255, 255, 255, 25));
    st.WindowRounding = 9.0f;
    st.FramePadding = ImVec2(5.0f, 5.0f);
    st.ItemSpacing = ImVec2(5.0f, 5.0f);
    st.WindowPadding = ImVec2(5.0f, 5.0f);
    st.Colors[ImGuiCol_WindowBg] = ImGui::ColorConvertU32ToFloat4(IM_COL32(20, 20, 20, 255));
    st.Colors[ImGuiCol_Text] = ImGui::ColorConvertU32ToFloat4(vars::Color::MCol3);
    ImGui_ImplWin32_Init(vars::misc::window::hwnd);
    ImGui_ImplDX11_Init(vars::d3d11::g_pd3dDevice, vars::d3d11::g_pd3dDeviceContext);

    img::LoadImageFromCompressed(&vars::Texture::MainLogoTexture, img::mainLogo_data, img::mainLogo_size, vars::d3d11::g_pd3dDevice);
    img::LoadImageFromCompressed(&vars::Texture::LogoWTexture, img::logo_data, img::logo_size, vars::d3d11::g_pd3dDevice);
    img::LoadImageFromCompressed(&vars::Texture::LogoBGTexture, img::logoBlur_data, img::logoBlur_size, vars::d3d11::g_pd3dDevice);
    img::LoadImageFromCompressed(&vars::Texture::BackGRTexture, img::whiteEffect_data, img::whiteEffect_size, vars::d3d11::g_pd3dDevice);
    img::LoadImageFromCompressed(&vars::Texture::CloseTexture, img::close_data, img::close_size, vars::d3d11::g_pd3dDevice);
    img::LoadImageFromCompressed(&vars::Texture::MinimTexture, img::minimize_data, img::minimize_size, vars::d3d11::g_pd3dDevice);
    img::LoadImageFromCompressed(&vars::Texture::TelegaTexture, img::telegram_data, img::telegram_size, vars::d3d11::g_pd3dDevice);
    img::LoadImageFromCompressed(&vars::Texture::BrowserTexture, img::browser_data, img::browser_size, vars::d3d11::g_pd3dDevice);

    bool done = false;

    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;
        if (vars::d3d11::g_SwapChainOccluded && vars::d3d11::g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        vars::d3d11::g_SwapChainOccluded = false;
        if (vars::d3d11::g_ResizeWidth != 0 && vars::d3d11::g_ResizeHeight != 0)
        {
            funcs::d3d11::CleanupRenderTarget();
            vars::d3d11::g_pSwapChain->ResizeBuffers(0, vars::d3d11::g_ResizeWidth, vars::d3d11::g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            vars::d3d11::g_ResizeWidth = vars::d3d11::g_ResizeHeight = 0;
            funcs::d3d11::CreateRenderTarget();
        }
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        if (vars::main::g_bindMode == 0 && !GetAsyncKeyState(vars::main::g_bindBtn) && !vars::Working::shouldStop) {
            std::unique_lock<std::mutex> lock(vars::Working::wkMtx);
            vars::Working::shouldStop = true;
        }
        
        if (vars::main::g_bindMode == 1 && vars::Working::ToggledBtn && vars::Working::canSwitchToggle) {
            if (GetAsyncKeyState(vars::main::g_bindBtn)) {
                std::unique_lock<std::mutex> lock(vars::Working::wkMtx);
                vars::Working::shouldStop = true;
                vars::Working::canSwitchToggle = false;
            }
        }

        ImGui::NewFrame();
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(io.DisplaySize);

            ui::Draw();

            HWND hwnd_f = GetForegroundWindow();
            if (funcs::usefull::IsMouseHoveringRect(vars::misc::dragArea::mMin, vars::misc::dragArea::mMax) || vars::Input::DraggingDragArea) {
                if (GetAsyncKeyState(VK_LBUTTON) &&
                    !vars::Working::working &&
                    vars::Input::canDragWindow &&
                    hwnd_f == vars::misc::window::hwnd &&
                    vars::Input::canDragWindow1) {
                    GetWindowRect(vars::misc::window::hwnd, &vars::misc::window::windowRect);
                    if (!vars::Input::DraggingDragArea) {
                        POINT mousePos;
                        GetCursorPos(&mousePos);
                        vars::Input::DragAreaOffset = ImVec2(mousePos.x - vars::misc::window::windowRect.left, mousePos.y - vars::misc::window::windowRect.top);
                        vars::Input::DraggingDragArea = true;
                    }
                    POINT mousePos;
                    GetCursorPos(&mousePos);
                    SetWindowPos(vars::misc::window::hwnd, HWND_TOPMOST, mousePos.x - vars::Input::DragAreaOffset.x, mousePos.y - vars::Input::DragAreaOffset.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
                }
                else {
                    vars::Input::DraggingDragArea = false;
                    vars::Input::DragAreaOffset = ImVec2(0, 0);
                }
            }
            else {
                vars::Input::DraggingDragArea = false;
                vars::Input::DragAreaOffset = ImVec2(0, 0);
            }
            if (vars::Input::MouseLock) {
                if (!GetAsyncKeyState(VK_LBUTTON)) {
                    vars::Input::MouseLock = false;
                }
            }
            if (!vars::Input::canWaitForKey) {
                if (!GetAsyncKeyState(VK_LBUTTON)) {
                    vars::Input::canWaitForKey = true;
                }
            }
            if (vars::Input::canWaitForKey) {
                if (vars::Input::waitingForKey1 || vars::Input::waitingForKey2) {
                    for (int i1 = 0; i1 <= 255; i1++) {
                        if (GetAsyncKeyState(i1)) {
                            vars::Working::CanWork = false;
                            if (vars::Input::waitingForKey1) {
                                if (i1 != vars::main::g_key) {
                                    vars::main::g_bindBtn = i1;
                                    vars::Input::waitingForKey1 = false;
                                    if (i1 == VK_LBUTTON) {
                                        vars::Input::MouseLock = true;
                                    }
                                }
                            }
                            if (vars::Input::waitingForKey2) {
                                if (i1 != vars::main::g_bindBtn) {
                                    vars::main::g_key = i1;
                                    vars::Input::waitingForKey2 = false;
                                    if (i1 == VK_LBUTTON) {
                                        vars::Input::MouseLock = true;
                                    }
                                }
                            }
                        }
                    }
                    vars::Working::canSwitchToggle = false;
                }
                if (!GetAsyncKeyState(vars::main::g_bindBtn) && !GetAsyncKeyState(vars::main::g_key)) {
                    vars::Working::CanWork = true;
                }
            }




        ImGui::Render();
        vars::d3d11::g_pd3dDeviceContext->OMSetRenderTargets(1, &vars::d3d11::g_mainRenderTargetView, nullptr);
        const float clear[4]{ 0.1f, 0.1f, 0.1f, 1.0f };
        vars::d3d11::g_pd3dDeviceContext->ClearRenderTargetView(vars::d3d11::g_mainRenderTargetView, clear);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        vars::d3d11::g_pSwapChain->Present(1, 0);
    }
    return 0;
}


