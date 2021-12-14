#pragma once

#include <imgui.h>
namespace MenuStyle {
	namespace Colors {
		static ImColor White(1.f, 1.f, 1.f, 1.f);
		static ImColor Black(0.f, 0.f, 0.f);
		static ImColor LighterBlue(.1f, .1f, 1.f);
		static ImColor LightBlue(.1f, .1f, .75f);
		static ImColor Blue(.1f, .1f, .5);
		static ImColor DarkBlue(.05f, .05f, .25f);
	}

	inline void Apply() {
		ImGuiStyle& Style = ImGui::GetStyle();

		Style.Colors[ImGuiCol_Button] = Colors::LightBlue;
		Style.Colors[ImGuiCol_ButtonHovered] = Colors::LighterBlue;
		Style.Colors[ImGuiCol_ButtonActive] = Colors::DarkBlue;
	}
}