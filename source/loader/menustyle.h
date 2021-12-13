#pragma once

#include <imgui.h>
namespace MenuStyle {
	inline void Apply() {
		ImGuiStyle& Style = ImGui::GetStyle();

		Style.WindowPadding = ImVec2(0.f, 0.f);
		Style.WindowBorderSize = 0.f;

		Style.Colors[ImGuiCol_Button] = ImVec4(.05f, .05f, .6f, 1.f);
	}
}