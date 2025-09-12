#pragma once
#include "../include/imgui/imgui.h"
#include "../variables/variables.h"
#include <string>

namespace gui {
	void pRECT(ImVec2 size);
	bool pBTN(ImVec2 size, vars::Type::btnType bt, int uniqueID = 1);
	bool pBTN_bind(float btnW, std::string text_d, std::string text_b, int uniqueID);
	bool pSLDR(float sldrW, std::string text_d, int value, int uniqueID, int v_min, int v_max, int* value_g);
}