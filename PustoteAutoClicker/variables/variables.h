#pragma once
#define col255to01(r, g, b, a) (ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f))
#define U8T(text) reinterpret_cast<const char*>(u8##text)
#include <d3d11.h>
#include "../include/imgui/imgui.h"
#include <string>
#include <condition_variable>
#include <mutex>

namespace vars {
	namespace main {
		inline int g_lang = 0; // 0 -> eng; 1 -> rus
		inline int g_bindBtn = 69;
		inline int g_key = 1;
		inline int g_holdDuration = 100;
		inline int g_clickInterval = 100;
		inline int g_bindMode = 0; // 0 -> hold; 1 -> toggle
	}
	namespace Texture {
		inline ID3D11ShaderResourceView* CloseTexture = nullptr;
		inline ID3D11ShaderResourceView* MinimTexture = nullptr;
		inline ID3D11ShaderResourceView* TelegaTexture = nullptr;
		inline ID3D11ShaderResourceView* BrowserTexture = nullptr;
		inline ID3D11ShaderResourceView* MainLogoTexture = nullptr;
		inline ID3D11ShaderResourceView* LogoBGTexture = nullptr;
		inline ID3D11ShaderResourceView* BackGRTexture = nullptr;
		inline ID3D11ShaderResourceView* LogoWTexture = nullptr;
	}
	namespace Color {
		inline ImU32 MCol1 = ImGui::ColorConvertFloat4ToU32(col255to01(100, 100, 100, 25));
		inline ImU32 MCol2 = ImGui::ColorConvertFloat4ToU32(col255to01(255, 255, 255, 10));
		inline ImU32 MCol3 = ImGui::ColorConvertFloat4ToU32(col255to01(255, 255, 255, 205));
		inline ImU32 MCol4 = ImGui::ColorConvertFloat4ToU32(col255to01(135, 135, 135, 50));
		inline ImU32 MCol5 = ImGui::ColorConvertFloat4ToU32(col255to01(50, 50, 50, 255));
	}
	namespace ui {
		inline std::string words[][2] = {
		{"Bind", U8T("Бинд")},
		{"Key", U8T("Клавиша")},
		{"Settings", U8T("Настройки")},
		{"Hold duration", U8T("Длит. зажатия")},
		{"Click interval", U8T("Интервал кликов")},
		{"Bind mode", U8T("Режим бинда")},
		{"Hold", U8T("Удерж.")},
		{"Toggle", U8T("Перекл.")},
		{"Version", U8T("Версия")},
		{"ENG", "RU"}
		};
	}
	namespace d3d11 {
		inline ID3D11Device* g_pd3dDevice = nullptr;
		inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
		inline IDXGISwapChain* g_pSwapChain = nullptr;
		inline bool g_SwapChainOccluded = false;
		inline UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
		inline ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
	}
	namespace Type {
		enum btnType {
			btn_default = 0,
			btn_lang = 1,
			btn_minimize = 2,
			btn_close = 3,
			btn_browser = 4,
			btn_telegram = 5,
			btn_discord = 6,
			btn_text = 7
		};
	}
	namespace Thread {
		inline bool g_ThreadShouldExit = false;
		inline HANDLE hThread = 0;
	}
	namespace Animation {
		inline float animSpeed = 5;

		inline float blurLogoOpacity = 0;

		inline float WelcomeBGopacity = 255;
		inline bool logoShown = false;
		inline bool EndWelcomeScreen = false;
		inline float WelcomeLogoOpacity = 0;
		inline float logoWOffset = 70.0f;

		inline float BGoffsetX = -550.0f;
		inline bool Needtr = false;
	}
	namespace Working {
		inline bool canSwitchToggle = true;
		inline bool CanWork = true;
		inline bool working = false;
		inline bool shouldStop = false;

		inline bool ToggledBtn = false;
	}
	namespace Input {
		inline bool DraggingDragArea = false;
		inline ImVec2 DragAreaOffset = { 0, 0 };
		inline bool canDragWindow = true;
		inline bool canDragWindow1 = true;

		inline bool waitingForKey1 = false;
		inline bool waitingForKey2 = false;

		inline bool canWaitForKey = true;

		inline bool MouseLock = false;

	}
	namespace misc {
		inline std::condition_variable cv;
		inline std::mutex cvMtx;
		namespace window {
			inline HWND hwnd;
			inline RECT windowRect;
		}
		
		namespace dragArea {
			inline ImVec2 mMin;
			inline ImVec2 mMax;
		}

	}
}
