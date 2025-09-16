#include "ui.h"
#include "../include/imgui/imgui.h"
#include "../variables/variables.h"
#include "../ui/ui_utils.h"
#include "../utils/utils.h"

namespace ui {
	void Draw() {

        ImGui::Begin("Pustote AutoClicker", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
        ImVec2 windowMin = ImGui::GetCursorScreenPos();
        ImVec2 windowMax = ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x, ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().y);
        if (vars::Animation::Needtr) {
            ImVec2 Posit = ImGui::GetCursorScreenPos();
            Posit.y -= 10.0f;
            Posit.x += vars::Animation::BGoffsetX;
            ImVec2 sizeA = ImGui::GetContentRegionAvail();
            sizeA.y += 30.0f;
            ImGui::GetWindowDrawList()->AddImage(vars::Texture::BackGRTexture, Posit, ImVec2(Posit.x + sizeA.x, Posit.y + sizeA.y), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 85));
            vars::Animation::BGoffsetX = funcs::usefull::ImLerp(vars::Animation::BGoffsetX, 550.0f, 2.0f);
            if (vars::Animation::BGoffsetX > 549.0f) {
                vars::Animation::Needtr = false;
            }
        }
        ImGui::BeginChild("LArea", ImVec2(150, 0));
        gui::pRECT(ImVec2(150, 150));
        ImGui::BeginChild("LogoPng", ImVec2(150, 150));

        ImVec2 CursPos = ImGui::GetCursorScreenPos();
        vars::Animation::blurLogoOpacity = funcs::usefull::ImLerp(vars::Animation::blurLogoOpacity, vars::Working::working ? 255 : 0, vars::Animation::animSpeed);
        ImGui::GetWindowDrawList()->AddImage(vars::Texture::LogoBGTexture, CursPos, ImVec2(CursPos.x + 150.0f, CursPos.y + 150.0f), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, vars::Animation::blurLogoOpacity));
        ImGui::GetWindowDrawList()->AddImage(vars::Texture::MainLogoTexture, CursPos, ImVec2(CursPos.x + 150.0f, CursPos.y + 150.0f));

        ImGui::EndChild();

        gui::pRECT(ImVec2(150, 0));
        ImGui::BeginChild("MoreArea", ImVec2(150, 0));

        float maxWidth = ImGui::GetContentRegionAvail().x / 3.0f - 6.66f;
        std::string vtxt = vars::ui::words[8][vars::main::g_lang] + ": 1.2";

        ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + (ImGui::GetContentRegionAvail().x / 2.0f) - (ImGui::CalcTextSize("Pustote AutoClicker").x / 2.0f), ImGui::GetCursorScreenPos().y + 5.0f), vars::Color::MCol3, "Pustote AutoClicker");
        ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + (ImGui::GetContentRegionAvail().x / 2.0f) - (ImGui::CalcTextSize(vtxt.c_str()).x / 2.0f), ImGui::GetCursorScreenPos().y + 15.0f + ImGui::CalcTextSize("Pustote AutoClicker").y), vars::Color::MCol3, vtxt.c_str());
        ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y - (25 + 10.0f)));
        ImGui::Dummy(ImVec2(0, 0));
        ImGui::SameLine();
        if (gui::pBTN(ImVec2(maxWidth, 25), vars::Type::btn_telegram, 91788881)) {
            funcs::usefull::openLink("https://t.me/pustote_official");
        }
        ImGui::SameLine();
        if (gui::pBTN(ImVec2(maxWidth, 25), vars::Type::btn_browser, 51989013)) {
            funcs::usefull::openLink("https://pustote.ct.ws/");
        }
        ImGui::SameLine();
        gui::pBTN(ImVec2(maxWidth, 25), vars::Type::btn_lang, 9178892);
        ImGui::EndChild();
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("RArea", ImVec2(0, 0), false);
        vars::misc::dragArea::mMin = ImGui::GetCursorScreenPos();
        vars::misc::dragArea::mMax = ImVec2(vars::misc::dragArea::mMin.x + ImGui::GetContentRegionAvail().x, vars::misc::dragArea::mMin.y + 30.0f);
        gui::pRECT(ImVec2(0, 30));
        ImGui::BeginChild("closeandmorearea", ImVec2(0, 30), ImGuiChildFlags_AlwaysUseWindowPadding);

        ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x - 50.0f, 1.0f));
        ImGui::SameLine();
        if (gui::pBTN(ImVec2(20.0f, 20.0f), vars::Type::btn_minimize, 871865)) {
            SendMessage(vars::misc::window::hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
        }
        ImGui::SameLine();
        if (gui::pBTN(ImVec2(20.0f, 20.0f), vars::Type::btn_close, 5892801)) {
            PostMessage(vars::misc::window::hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
        }
        ImGui::EndChild();
        GetWindowRect(vars::misc::window::hwnd, &vars::misc::window::windowRect);
        vars::misc::dragArea::mMin = ImVec2(vars::misc::window::windowRect.left + vars::misc::dragArea::mMin.x, vars::misc::window::windowRect.top + vars::misc::dragArea::mMin.y);
        vars::misc::dragArea::mMax = ImVec2(vars::misc::window::windowRect.left + vars::misc::dragArea::mMax.x, vars::misc::window::windowRect.top + vars::misc::dragArea::mMax.y);
        gui::pRECT(ImGui::GetContentRegionAvail());
        ImGui::BeginChild("MainSettingsArea", ImVec2(0, 0), ImGuiChildFlags_AlwaysUseWindowPadding);

        gui::pRECT(ImVec2(0, 30));
        ImGui::BeginChild("BindA", ImVec2(0, 30));

        if (gui::pBTN_bind(150.0f, vars::ui::words[0][vars::main::g_lang], vars::Input::waitingForKey1 ? "..." : funcs::usefull::KeyCodeToString(vars::main::g_bindBtn).c_str(), 19578198)) {
            vars::Input::waitingForKey1 = true;
            vars::Input::waitingForKey2 = false;
            vars::Input::canWaitForKey = false;
        }
        ImGui::EndChild();

        gui::pRECT(ImVec2(0, 30));
        ImGui::BeginChild("KeyA", ImVec2(0, 30));

        if (gui::pBTN_bind(150.0f, vars::ui::words[1][vars::main::g_lang], vars::Input::waitingForKey2 ? "..." : funcs::usefull::KeyCodeToString(vars::main::g_key).c_str(), 6549871)) {
            vars::Input::waitingForKey1 = false;
            vars::Input::waitingForKey2 = true;
            vars::Input::canWaitForKey = false;
        }
        ImGui::EndChild();

        gui::pRECT(ImVec2(0, 0));
        ImGui::BeginChild("SettingsA", ImVec2(0, ImGui::GetContentRegionAvail().y - 5.0f), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImGui::Text(vars::ui::words[2][vars::main::g_lang].c_str());

        gui::pRECT(ImVec2(0, 30.0f));
        ImGui::BeginChild("HoldDurA", ImVec2(0, 30.0f));

        if (gui::pSLDR(175.0f, vars::ui::words[3][vars::main::g_lang], vars::main::g_holdDuration, 18579167, 1, 1500, &vars::main::g_holdDuration)) {
            vars::Input::canDragWindow1 = false;
        }
        else {
            vars::Input::canDragWindow1 = true;
        }
        ImGui::EndChild();

        gui::pRECT(ImVec2(0, 30.0f));
        ImGui::BeginChild("ClickInterA", ImVec2(0, 30.0f));

        if (gui::pSLDR(175.0f, vars::ui::words[4][vars::main::g_lang], vars::main::g_clickInterval, 17849183, 1, 1500, &vars::main::g_clickInterval)) {
            vars::Input::canDragWindow1 = false;
        }

        ImGui::EndChild();

        gui::pRECT(ImVec2(0, 30.0f));
        ImGui::BeginChild("BindModeA", ImVec2(0, 30.0f));

        if (gui::pBTN_bind(150.0f, vars::ui::words[5][vars::main::g_lang], vars::ui::words[6 + vars::main::g_bindMode][vars::main::g_lang], 86875564)) {
            if (vars::main::g_bindMode < 1) {
                vars::main::g_bindMode += 1;
            }
            else {
                vars::main::g_bindMode = 0;
            }
        }
        ImGui::EndChild();
        ImGui::EndChild();
        ImGui::EndChild();
        ImGui::EndChild();
        ImGui::GetForegroundDrawList()->AddRectFilled(windowMin, windowMax, IM_COL32(20, 20, 20, (int)vars::Animation::WelcomeBGopacity));
        ImVec2 CenterW = ImVec2(windowMin.x + (windowMax.x - windowMin.x) / 2.0f, windowMin.y + (windowMax.y - windowMin.y) / 2.0f);
        ImVec2 logoMin = ImVec2(CenterW.x - vars::Animation::logoWOffset, CenterW.y - vars::Animation::logoWOffset);
        ImVec2 logoMax = ImVec2(CenterW.x + vars::Animation::logoWOffset, CenterW.y + vars::Animation::logoWOffset);
        if (!vars::Animation::EndWelcomeScreen) {
            ImGui::GetForegroundDrawList()->AddImage(vars::Texture::LogoWTexture, logoMin, logoMax, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, vars::Animation::WelcomeLogoOpacity));
            vars::Animation::WelcomeLogoOpacity = funcs::usefull::ImLerp(vars::Animation::WelcomeLogoOpacity, vars::Animation::logoShown ? 0 : 255, vars::Animation::logoShown ? 4.0f : 3.0f);
            vars::Animation::logoWOffset = funcs::usefull::ImLerp(vars::Animation::logoWOffset, vars::Animation::logoShown ? 70.0f : 72.5f, 3.0f);
            if (vars::Animation::WelcomeLogoOpacity > 250.0f) {
                vars::Animation::logoShown = true;
            }
            if (vars::Animation::logoShown && vars::Animation::WelcomeLogoOpacity < 1.0f) {
                vars::Animation::EndWelcomeScreen = true;
            }
        }
        if (vars::Animation::WelcomeBGopacity > 0.2f && vars::Animation::EndWelcomeScreen) {
            vars::Animation::WelcomeBGopacity = funcs::usefull::ImLerp(vars::Animation::WelcomeBGopacity, 0, 2.0f);
        }


        ImGui::End();
	}
}