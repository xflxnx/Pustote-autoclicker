#include <Windows.h>
#include "variables/variables.h"


DWORD WINAPI MainThread(LPVOID lpParam) {
    while (!vars::Thread::g_ThreadShouldExit) {
        INPUT inp = {};
        INPUT inp2 = {};
        switch (vars::main::g_key) {
        case VK_LBUTTON:
            inp.type = INPUT_MOUSE;
            inp2.type = INPUT_MOUSE;
            inp.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            inp2.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            inp.mi.dwExtraInfo = 4919;
            inp2.mi.dwExtraInfo = 4919;
            break;
        case VK_RBUTTON:
            inp.type = INPUT_MOUSE;
            inp2.type = INPUT_MOUSE;
            inp.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            inp2.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            inp.mi.dwExtraInfo = 4919;
            inp2.mi.dwExtraInfo = 4919;
            break;
        case VK_MBUTTON:
            inp.type = INPUT_MOUSE;
            inp2.type = INPUT_MOUSE;
            inp.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            inp2.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            inp.mi.dwExtraInfo = 4919;
            inp2.mi.dwExtraInfo = 4919;
            break;
        case VK_XBUTTON1:
            inp.type = INPUT_MOUSE;
            inp2.type = INPUT_MOUSE;
            inp.mi.dwFlags = MOUSEEVENTF_XDOWN;
            inp2.mi.dwFlags = MOUSEEVENTF_XUP;
            inp.mi.mouseData = XBUTTON1;
            inp2.mi.mouseData = XBUTTON1;
            inp.mi.dwExtraInfo = 4919;
            inp2.mi.dwExtraInfo = 4919;
            break;
        case VK_XBUTTON2:
            inp.type = INPUT_MOUSE;
            inp2.type = INPUT_MOUSE;
            inp.mi.dwFlags = MOUSEEVENTF_XDOWN;
            inp2.mi.dwFlags = MOUSEEVENTF_XUP;
            inp.mi.mouseData = XBUTTON2;
            inp2.mi.mouseData = XBUTTON2;
            inp.mi.dwExtraInfo = 4919;
            inp2.mi.dwExtraInfo = 4919;
            break;
        default:
            inp.type = INPUT_KEYBOARD;
            inp2.type = INPUT_KEYBOARD;
            inp.ki.wVk = vars::main::g_key;
            inp2.ki.wVk = vars::main::g_key;
            inp.ki.wScan = MapVirtualKey(vars::main::g_key, 0);
            inp2.ki.wScan = MapVirtualKey(vars::main::g_key, 0);
            inp.ki.dwFlags = WM_KEYDOWN;
            inp2.ki.dwFlags = KEYEVENTF_KEYUP;
            inp.ki.dwExtraInfo = 4919;
            inp2.ki.dwExtraInfo = 4919;
            break;
        }

        if (vars::main::g_bindMode == 1) {
            std::unique_lock<std::mutex> lock(vars::Working::wkMtx);
            if (GetAsyncKeyState(vars::main::g_bindBtn) && 
            vars::Working::canSwitchToggle) {
                
                vars::Working::ToggledBtn = !vars::Working::ToggledBtn;
                vars::Working::canSwitchToggle = false;
            }
            if (!GetAsyncKeyState(vars::main::g_bindBtn) &&
                !vars::Working::canSwitchToggle) {
                vars::Working::canSwitchToggle = true;
            }
        }

        if (vars::main::g_bindMode == 0) {
            if (GetAsyncKeyState(vars::main::g_bindBtn) && vars::Working::CanWork) {
                vars::Working::working = true;
                std::unique_lock<std::mutex> lock(vars::misc::cvMtx);
                SendInput(1, &inp, sizeof(inp));
                vars::misc::cv.wait_for(lock, std::chrono::milliseconds(vars::main::g_holdDuration), [] {return vars::Working::shouldStop; });
                SendInput(1, &inp2, sizeof(inp2));
                vars::misc::cv.wait_for(lock, std::chrono::milliseconds(vars::main::g_clickInterval), [] {return vars::Working::shouldStop; });
            }
            else {
                vars::Working::working = false;
            }
        }
        else {
            if (vars::Working::ToggledBtn) {
                vars::Working::working = true;
                std::unique_lock<std::mutex> lock(vars::misc::cvMtx);
                SendInput(1, &inp, sizeof(inp));
                
                vars::misc::cv.wait_for(lock, std::chrono::milliseconds(vars::main::g_holdDuration), [] {return vars::Working::shouldStop; });
                SendInput(1, &inp2, sizeof(inp2));
                if (vars::main::g_clickInterval > 1) {
                    vars::misc::cv.wait_for(lock, std::chrono::milliseconds(vars::main::g_clickInterval), [] {return vars::Working::shouldStop; });
                }
                if (vars::Working::shouldStop) {
                    vars::Working::ToggledBtn = false;
                    vars::Working::shouldStop = false;
                }
            }
            else {
                vars::Working::working = false;
            }
        }

        Sleep(1);
    }
    return 0;
}