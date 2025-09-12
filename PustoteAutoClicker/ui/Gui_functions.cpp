#define _CRT_SECURE_NO_WARNINGS
#include "../include/imgui/imgui.h"
#include "../variables/variables.h"
#include "../functions/Functions.h"
#include "../include/imgui/imgui_internal.h"

namespace gui {
    void pRECT(ImVec2 size) {
        ImVec2 Pos = ImGui::GetCursorScreenPos();

        if (size.x == 0) size.x = ImGui::GetContentRegionAvail().x;
        if (size.y == 0) size.y = ImGui::GetContentRegionAvail().y;

        ImGui::GetWindowDrawList()->AddRectFilled(Pos, ImVec2(Pos.x + size.x, Pos.y + size.y), vars::Color::MCol1, 9.0f);
        ImGui::GetWindowDrawList()->AddRect(Pos, ImVec2(Pos.x + size.x, Pos.y + size.y), vars::Color::MCol2, 9.0f, 0, 1.0f);
    }

    bool pBTN(ImVec2 size, vars::Type::btnType bt, int uniqueID = 1) {
        ImVec2 Pos = ImGui::GetCursorScreenPos();

        bool IsHovering = ImGui::IsMouseHoveringRect(Pos, ImVec2(Pos.x + size.x, Pos.y + size.y));

        ImGuiStorage* strg = ImGui::GetStateStorage();
        ImGuiID Clr1_k = ImGui::GetID(uniqueID);
        ImGuiID Clr2_k = ImGui::GetID(uniqueID + 5);

        float Clr1_f = strg->GetFloat(Clr1_k, IsHovering ? 100.0f : 135.0f); // 100 -> 135
        float Clr2_f = strg->GetFloat(Clr2_k, IsHovering ? 25.0f : 50.0f); // 25 -> 50

        if (IsHovering) {
            if (bt == vars::Type::btn_close || bt == vars::Type::btn_minimize) {
                if (!vars::Input::DraggingDragArea) {
                    vars::Input::canDragWindow = false;
                }
            }

            if (GetAsyncKeyState(VK_LBUTTON)) {
                IsHovering = false;
            }
            Clr1_f = funcs::usefull::ImLerp(Clr1_f, 135.0f, vars::Animation::animSpeed);
            Clr2_f = funcs::usefull::ImLerp(Clr2_f, 50.0f, vars::Animation::animSpeed);
            if (Clr1_f >= 134.9f) {
                Clr1_f = 135.0f;
            }
            if (Clr2_f >= 49.9f) {
                Clr2_f = 50.0f;
            }
        }
        if (!IsHovering) {
            if (bt == vars::Type::btn_close || bt == vars::Type::btn_minimize) {
                if (!GetAsyncKeyState(VK_LBUTTON)) {
                    vars::Input::canDragWindow = true;
                }
            }
            Clr1_f = funcs::usefull::ImLerp(Clr1_f, 100.0f, vars::Animation::animSpeed);
            Clr2_f = funcs::usefull::ImLerp(Clr2_f, 25.0f, vars::Animation::animSpeed);
            if (Clr1_f <= 100.1f) {
                Clr1_f = 100.0f;
            }
            if (Clr2_f <= 25.1f) {
                Clr2_f = 25.0f;
            }
        }
        strg->SetFloat(Clr1_k, Clr1_f);
        strg->SetFloat(Clr2_k, Clr2_f);

        ImU32 f_Clr = ImGui::ColorConvertFloat4ToU32(col255to01((int)Clr1_f, (int)Clr1_f, (int)Clr1_f, (int)Clr2_f));
        ImGui::GetWindowDrawList()->AddRectFilled(Pos, ImVec2(Pos.x + size.x, Pos.y + size.y), f_Clr, 6.0f);
        ImGui::GetWindowDrawList()->AddRect(Pos, ImVec2(Pos.x + size.x, Pos.y + size.y), vars::Color::MCol2, 6.0f, 0, 1.0f);
        if (bt == vars::Type::btn_close) {
            ImGui::GetWindowDrawList()->AddImage(vars::Texture::CloseTexture, ImVec2(Pos), ImVec2(Pos.x + size.x, Pos.y + size.y));
        }
        if (bt == vars::Type::btn_minimize) {
            ImGui::GetWindowDrawList()->AddImage(vars::Texture::MinimTexture, ImVec2(Pos), ImVec2(Pos.x + size.x, Pos.y + size.y));
        }
        if (bt == vars::Type::btn_telegram) {
            ImGui::GetWindowDrawList()->AddImage(vars::Texture::TelegaTexture, ImVec2(Pos), ImVec2(Pos.x + size.x, Pos.y + size.y));
        }
        if (bt == vars::Type::btn_browser) {
            ImGui::GetWindowDrawList()->AddImage(vars::Texture::BrowserTexture, ImVec2(Pos), ImVec2(Pos.x + size.x, Pos.y + size.y));
        }

        if (bt == vars::Type::btn_lang) {
            std::string langName = vars::ui::words[9][vars::main::g_lang];
            ImGui::GetWindowDrawList()->AddText(ImVec2(Pos.x + (size.x / 2.0f) - ImGui::CalcTextSize(langName.c_str()).x / 2.0f, Pos.y + (size.y / 2.0f) - ImGui::CalcTextSize(langName.c_str()).y / 2.0f), vars::Color::MCol3, langName.c_str());
        }
        if (ImGui::InvisibleButton(std::to_string(uniqueID).c_str(), size)) {
            if (bt == vars::Type::btn_lang) {
                if (vars::main::g_lang < 1) {
                    vars::main::g_lang++;
                }
                else {
                    vars::main::g_lang--;
                }
                vars::Animation::Needtr = true;
                vars::Animation::BGoffsetX = -550.0f;
            }
            return true;
        }
        return false;
    }

    bool pBTN_bind(float btnW, std::string text_d, std::string text_b, int uniqueID) {
        ImVec2 Maxs = ImGui::GetContentRegionAvail();
        ImVec2 textPos = ImVec2(ImGui::GetCursorScreenPos().x + 5.0f, ImGui::GetCursorScreenPos().y + (Maxs.y / 2.0f) - (ImGui::CalcTextSize(text_d.c_str()).y / 2.0f));
        ImVec2 btnMin = ImVec2(ImGui::GetCursorScreenPos().x + Maxs.x - (btnW + 5.0f), ImGui::GetCursorScreenPos().y + 5.0f);
        ImVec2 btnMax = ImVec2(ImGui::GetCursorScreenPos().x + Maxs.x - 5.0f, ImGui::GetCursorScreenPos().y + Maxs.y - 5.0f);
        ImVec2 textPos2 = ImVec2(btnMin.x + (btnMax.x - btnMin.x) / 2.0f - ImGui::CalcTextSize(text_b.c_str()).x / 2.0f, btnMin.y + (btnMax.y - btnMin.y) / 2.0f - ImGui::CalcTextSize(text_b.c_str()).y / 2.0f);
        bool IsHovering = ImGui::IsMouseHoveringRect(btnMin, btnMax);

        ImGuiStorage* strg = ImGui::GetStateStorage();
        ImGuiID Clr1_k = ImGui::GetID(uniqueID);
        ImGuiID Clr2_k = ImGui::GetID(uniqueID + 5);

        float Clr1_f = strg->GetFloat(Clr1_k, IsHovering ? 100.0f : 135.0f); // 100 -> 135
        float Clr2_f = strg->GetFloat(Clr2_k, IsHovering ? 25.0f : 50.0f); // 25 -> 50

        if (IsHovering) {
            if (GetAsyncKeyState(VK_LBUTTON)) {
                IsHovering = false;
            }
            Clr1_f = funcs::usefull::ImLerp(Clr1_f, 135.0f, vars::Animation::animSpeed);
            Clr2_f = funcs::usefull::ImLerp(Clr2_f, 50.0f, vars::Animation::animSpeed);
            if (Clr1_f >= 134.9f) {
                Clr1_f = 135.0f;
            }
            if (Clr2_f >= 49.9f) {
                Clr2_f = 50.0f;
            }
        }
        if (!IsHovering) {
            Clr1_f = funcs::usefull::ImLerp(Clr1_f, 100.0f, vars::Animation::animSpeed);
            Clr2_f = funcs::usefull::ImLerp(Clr2_f, 25.0f, vars::Animation::animSpeed);
            if (Clr1_f <= 100.1f) {
                Clr1_f = 100.0f;
            }
            if (Clr2_f <= 25.1f) {
                Clr2_f = 25.0f;
            }
        }
        strg->SetFloat(Clr1_k, Clr1_f);
        strg->SetFloat(Clr2_k, Clr2_f);

        ImU32 f_Clr = ImGui::ColorConvertFloat4ToU32(col255to01((int)Clr1_f, (int)Clr1_f, (int)Clr1_f, (int)Clr2_f));
        ImGui::GetWindowDrawList()->AddRectFilled(btnMin, btnMax, f_Clr, 9.0f);
        ImGui::GetWindowDrawList()->AddRect(btnMin, btnMax, vars::Color::MCol2, 9.0f, 0, 1.0f);
        ImGui::GetWindowDrawList()->AddText(textPos, vars::Color::MCol3, text_d.c_str());
        ImGui::GetWindowDrawList()->AddText(textPos2, vars::Color::MCol3, text_b.c_str());
        ImGui::SetCursorScreenPos(btnMin);
        Maxs = ImGui::GetContentRegionAvail();
        if (ImGui::InvisibleButton(std::to_string(uniqueID).c_str(), ImVec2(Maxs.x - 5.0f, Maxs.y - 5.0f)) && !vars::Input::MouseLock) {
            for (int i = 0; i < 255; i++) {
                GetAsyncKeyState(i);
            }
            return true;
        }
        return false;
    }

    bool pSLDR(float sldrW, std::string text_d, int value, int uniqueID, int v_min, int v_max, int* value_g) {
        bool returnval = false;
        ImVec2 Maxs = ImGui::GetContentRegionAvail();

        ImVec2 textPos = ImVec2(ImGui::GetCursorScreenPos().x + 5.0f, ImGui::GetCursorScreenPos().y + (Maxs.y / 2.0f) - (ImGui::CalcTextSize(text_d.c_str()).y / 2.0f));

        ImVec2 sldrMin = ImVec2(ImGui::GetCursorScreenPos().x + (Maxs.x - (60.0f + sldrW)), ImGui::GetCursorScreenPos().y + 10.0f);
        ImVec2 sldrMax = ImVec2(ImGui::GetCursorScreenPos().x + (Maxs.x - 60.0f), ImGui::GetCursorScreenPos().y + Maxs.y - 10.0f);

        ImVec2 vDispMin = ImVec2(ImGui::GetCursorScreenPos().x + (Maxs.x - 55.0f), ImGui::GetCursorScreenPos().y + 5.0f);
        ImVec2 vDispMax = ImVec2(ImGui::GetCursorScreenPos().x + (Maxs.x - 5.0f), ImGui::GetCursorScreenPos().y + Maxs.y - 5.0f);

        ImVec2 hPos = ImVec2(funcs::usefull::MapValue(value, v_min, v_max, (int)sldrMin.x + 5, (int)sldrMax.x - 5), sldrMin.y + (sldrMax.y - sldrMin.y) / 2.0f);

        ImVec2 DispTextPos = ImVec2(vDispMin.x + 5.0f, vDispMin.y + (vDispMax.y - vDispMin.y) / 2.0f - (ImGui::CalcTextSize(std::to_string(value).c_str()).y / 2.0f));

        bool IsHovering = ImGui::IsMouseHoveringRect(sldrMin, sldrMax);

        ImGuiStorage* strg = ImGui::GetStateStorage();
        ImGuiID Clr1_k = ImGui::GetID(uniqueID);
        ImGuiID Clr2_k = ImGui::GetID(uniqueID + 5);

        float Clr1_f = strg->GetFloat(Clr1_k, IsHovering ? 100.0f : 135.0f); // 100 -> 135
        float Clr2_f = strg->GetFloat(Clr2_k, IsHovering ? 25.0f : 50.0f); // 25 -> 50

        if (IsHovering) {
            Clr1_f = funcs::usefull::ImLerp(Clr1_f, 135.0f, vars::Animation::animSpeed);
            Clr2_f = funcs::usefull::ImLerp(Clr2_f, 50.0f, vars::Animation::animSpeed);
            if (Clr1_f >= 134.9f) {
                Clr1_f = 135.0f;
            }
            if (Clr2_f >= 49.9f) {
                Clr2_f = 50.0f;
            }
        }
        if (!IsHovering) {
            Clr1_f = funcs::usefull::ImLerp(Clr1_f, 100.0f, vars::Animation::animSpeed);
            Clr2_f = funcs::usefull::ImLerp(Clr2_f, 25.0f, vars::Animation::animSpeed);
            if (Clr1_f <= 100.1f) {
                Clr1_f = 100.0f;
            }
            if (Clr2_f <= 25.1f) {
                Clr2_f = 25.0f;
            }
        }
        strg->SetFloat(Clr1_k, Clr1_f);
        strg->SetFloat(Clr2_k, Clr2_f);

        ImU32 f_Clr = ImGui::ColorConvertFloat4ToU32(col255to01((int)Clr1_f, (int)Clr1_f, (int)Clr1_f, (int)Clr2_f));

        bool IsHovering2 = ImGui::IsMouseHoveringRect(vDispMin, vDispMax);

        ImGuiID Clr3_k = ImGui::GetID(uniqueID + 10);
        ImGuiID Clr4_k = ImGui::GetID(uniqueID + 15);

        float Clr3_f = strg->GetFloat(Clr3_k, IsHovering2 ? 100.0f : 135.0f); // 100 -> 135
        float Clr4_f = strg->GetFloat(Clr4_k, IsHovering2 ? 25.0f : 50.0f); // 25 -> 50

        if (IsHovering2) {
            Clr3_f = funcs::usefull::ImLerp(Clr3_f, 135.0f, vars::Animation::animSpeed);
            Clr4_f = funcs::usefull::ImLerp(Clr4_f, 50.0f, vars::Animation::animSpeed);
            if (Clr3_f >= 134.9f) {
                Clr3_f = 135.0f;
            }
            if (Clr4_f >= 49.9f) {
                Clr4_f = 50.0f;
            }
        }
        if (!IsHovering2) {
            Clr3_f = funcs::usefull::ImLerp(Clr3_f, 100.0f, vars::Animation::animSpeed);
            Clr4_f = funcs::usefull::ImLerp(Clr4_f, 25.0f, vars::Animation::animSpeed);
            if (Clr3_f <= 100.1f) {
                Clr3_f = 100.0f;
            }
            if (Clr4_f <= 25.1f) {
                Clr4_f = 25.0f;
            }
        }
        strg->SetFloat(Clr3_k, Clr3_f);
        strg->SetFloat(Clr4_k, Clr4_f);

        ImU32 f_Clr2 = ImGui::ColorConvertFloat4ToU32(col255to01((int)Clr3_f, (int)Clr3_f, (int)Clr3_f, (int)Clr4_f));

        ImGui::SetCursorScreenPos(ImVec2(vDispMin.x + 5.0f, vDispMin.y + 2.5f));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 5.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(0, 0, 0, 0));
        char buf[5] = "";
        strcpy(buf, std::to_string(value).c_str());

        std::string outIntS = "";

        bool InputTextActive = false;

        ImGui::InputText(std::to_string(uniqueID).c_str(), buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_ParseEmptyRefVal);
        if (ImGui::IsItemActiveAsInputText()) {
            InputTextActive = true;
            for (char a : buf) {
                if (a == '0' || a == '1' || a == '2' || a == '3' || a == '4' || a == '5' || a == '6' || a == '7' || a == '8' || a == '9') {
                    outIntS += a;
                }
            }
            if (!outIntS.empty()) {
                int outInt = std::stoi(outIntS);
                if (outInt <= 1500) {
                    *value_g = outInt;
                }
            }
            if (outIntS.empty()) {
                *value_g = 0;
            }
        }
        else {
            InputTextActive = false;
        }

        ImGui::PopItemWidth();
        ImGui::PopStyleColor(7);
        ImGui::PopStyleVar(3);

        ImGuiID Dragging_k = ImGui::GetID(uniqueID + 25);
        bool dragging = strg->GetFloat(Dragging_k, false);
        if (IsHovering || dragging) {
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && !InputTextActive) {
                dragging = true;
                returnval = true;
                ImVec2 cursPs = ImGui::GetMousePos();
                if (cursPs.x <= sldrMin.x + 5.0f) {
                    hPos.x = sldrMin.x + 5.0f;
                }
                if (cursPs.x >= sldrMax.x - 5.0f) {
                    hPos.x = sldrMax.x - 5.0f;
                }
                if (cursPs.x <= sldrMax.x - 5.0f && cursPs.x >= sldrMin.x + 5.0f) {
                    hPos.x = cursPs.x;
                }
                *value_g = funcs::usefull::MapValue((int)hPos.x, (int)sldrMin.x + 5, (int)sldrMax.x - 5, v_min, v_max);
            }
            else {
                dragging = false;
                returnval = false;
            }
        }
        else {
            dragging = false;
            returnval = false;
        }
        strg->SetFloat(Dragging_k, dragging);

        ImGui::GetWindowDrawList()->AddRectFilled(sldrMin, sldrMax, f_Clr, 10.0f);
        ImGui::GetWindowDrawList()->AddRect(sldrMin, sldrMax, vars::Color::MCol2, 10.0f, 0, 1.0f);
        ImGui::GetWindowDrawList()->AddText(textPos, vars::Color::MCol3, text_d.c_str());

        ImGui::GetWindowDrawList()->AddRectFilled(vDispMin, vDispMax, f_Clr2, 10.0f);
        ImGui::GetWindowDrawList()->AddRect(vDispMin, vDispMax, vars::Color::MCol2, 10.0f, 0, 1.0f);
        ImGui::GetWindowDrawList()->AddText(DispTextPos, vars::Color::MCol3, std::to_string(value).c_str());

        ImGui::GetWindowDrawList()->AddCircleFilled(hPos, 6.0f, f_Clr);
        ImGui::GetWindowDrawList()->AddCircle(hPos, 6.0f, vars::Color::MCol2);
        return returnval;
    }
}