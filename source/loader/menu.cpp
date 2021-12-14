/*
	to-do:
		write a good looking menu, the current one doesn't look good at all
		write a proper window drag system (current one has tons of lag)
*/

#include "menu.h"
#include "window.h"
#include "game.h"
#include "menustyle.h"
#include "globals.h"
#include "util/resources.h"

#include <mutex>

#include <imgui.h>
#include <imgui_internal.h>

float fTopbarHeight;
float fTopbarPadding;
float fIconPadding;
float fPadding;

char cBuildText[18];

ImVec2 vTitleSize;
ImVec2 vBuildTextSize;
ImVec2 vButtonSize;
ImVec2 vIconSize;
ImVec2 vTabSize;

ImVec2 vZero2(0, 0);

std::once_flag fMenuInitialized;

ImGuiContext* Ctx;

unsigned int IconTex;

int iCurrentTab = 0;

void InitMenu() {
	fPadding = Window::iSizeH / 50.f;

	vButtonSize = ImVec2((Window::iSizeW - (fPadding * 5)) / 4.f, Window::iSizeH / 10.f);

	sprintf(cBuildText, "%s build", g_Build);

	vTitleSize = ImGui::CalcTextSize("sledge");
	vBuildTextSize = ImGui::CalcTextSize(cBuildText);


	fTopbarPadding = (vTitleSize.y + vBuildTextSize.y) * 0.1f;
	fTopbarHeight = (vTitleSize.y + vBuildTextSize.y) + (fTopbarPadding * 3.f);

	vTitleSize = ImVec2(vTitleSize.x / 2, vTitleSize.y);
	vBuildTextSize = ImVec2(vBuildTextSize.x / 2, vBuildTextSize.y);

	Ctx = ImGui::GetCurrentContext();
	MenuStyle::Apply();

	IconTex = Resources::TextureFromPNG("ICON_PNG");

	fIconPadding = fPadding * 15;
	float fIconSize = Window::iSizeW - (fIconPadding * 2);

	vIconSize = ImVec2(fIconSize, fIconSize);

	vTabSize = ImVec2(Window::iSizeW - (fPadding * 2), Window::iSizeH - (fPadding * 2) - (vButtonSize.y * 2));

	ImGui::SetNextWindowSize(ImVec2(Window::iSizeW, Window::iSizeH));
}

void Menu::Draw() {
	std::call_once(fMenuInitialized, InitMenu);

	ImGui::SetNextWindowPos(vZero2);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, vZero2);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

	ImGui::Begin("menu", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	ImGui::GetWindowDrawList()->AddRectFilled(vZero2, ImVec2(Window::iSizeW, fTopbarHeight), MenuStyle::Colors::LightBlue);

	ImGui::GetWindowDrawList()->AddText(ImVec2((Window::iSizeW / 2.f) - (vTitleSize.x), fTopbarPadding), MenuStyle::Colors::White, "sledge");
	ImGui::GetWindowDrawList()->AddText(ImVec2((Window::iSizeW / 2.f) - (vBuildTextSize.x), (fTopbarPadding * 2) + vTitleSize.y), MenuStyle::Colors::White, cBuildText);

	ImGui::SetCursorPosX(Window::iSizeW - fTopbarHeight);
	if (ImGui::Button("X", ImVec2(fTopbarHeight, fTopbarHeight)))
		Window::Close();

	switch (iCurrentTab) {
	case 0:
		ImGui::SetCursorPos(ImVec2(fIconPadding, (fIconPadding / 4) + fTopbarHeight));
		ImGui::Image((void*)(intptr_t)IconTex, vIconSize);
		break;
	case 1:
		ImGui::SetCursorPos(ImVec2(fPadding, fPadding + fTopbarHeight));

		ImGui::BeginChild("modlist", vTabSize);
			ImGui::Text("hi");
			ImGui::Button("wtf");
		ImGui::EndChild();

		break;
	}

	ImGui::SetCursorPosY(Window::iSizeH - fPadding - vButtonSize.y);
	ImGui::SetCursorPosX(fPadding);
	if (ImGui::Button("play", vButtonSize))
		if (Game::Launch()) 
			Window::Close();
	
	ImGui::SameLine();
	ImGui::SetCursorPosX((fPadding * 2) + vButtonSize.x);
	if (ImGui::Button("mods", vButtonSize)) {
		iCurrentTab = 1;
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX((fPadding * 3) + (vButtonSize.x * 2));
	if (ImGui::Button("settings", vButtonSize)) {
		iCurrentTab = 2;
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX((fPadding * 4) + (vButtonSize.x * 3));
	if (ImGui::Button("discord", vButtonSize)) {
		iCurrentTab = 3;
	}

	ImGui::End();

	/*
		quick hack to get window dragging working
	*/
	if (Ctx->MovingWindow) {
		ImVec2 vMouseDrag = ImGui::GetMouseDragDelta();
		
		// we don't really care for 4244 here
		#pragma warning(push)
		#pragma warning(disable: 4244)
		Window::MoveWindow(vMouseDrag.x, vMouseDrag.y);
		#pragma warning(pop)
	}
}