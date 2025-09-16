#pragma once
#include "../utils/utils.h"
#include "../include/imgui/imgui.h"
#include <d3d11.h>
#include <string>

namespace funcs {
    namespace d3d11 {
        bool CreateDeviceD3D(HWND hWnd);
        void CleanupDeviceD3D();
        void CreateRenderTarget();
        void CleanupRenderTarget();
        LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    }
    namespace usefull {
        bool openLink(std::string link);
        template <typename T> T MapValue(T value, T in_min, T in_max, T out_min, T out_max) {
            return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }
        float ImLerp(float current, float target, float speed);
        std::string KeyCodeToString(int keyCode);
        bool IsMouseHoveringRect(ImVec2 rMin, ImVec2 rMax);
    }
}

