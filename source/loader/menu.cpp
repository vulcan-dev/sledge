/*
	to-do:
		write a good looking menu, the current one doesn't look good at all
		write a proper window drag system (current one has tons of lag)
*/

#include "menu.h"
#include "window.h"
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

ImColor ImCol_White(1.f, 1.f, 1.f, 1.f);

std::once_flag fMenuInitialized;

ImGuiContext* Ctx;

unsigned int IconTex;


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

	ImGui::SetNextWindowSize(ImVec2(Window::iSizeW, Window::iSizeH));
}

void Menu::Draw() {
	std::call_once(fMenuInitialized, InitMenu);

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("menu", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0.f, 0.f), ImVec2(Window::iSizeW, fTopbarHeight), ImColor(.1f, .1f, .75f));
	
	ImGui::GetWindowDrawList()->AddText(ImVec2((Window::iSizeW / 2.f) - (vTitleSize.x), fTopbarPadding), ImCol_White, "sledge");
	ImGui::GetWindowDrawList()->AddText(ImVec2((Window::iSizeW / 2.f) - (vBuildTextSize.x), (fTopbarPadding * 2) + vTitleSize.y), ImCol_White, cBuildText);

	ImGui::SetCursorPosX(Window::iSizeW - fTopbarHeight);
	if (ImGui::Button("X", ImVec2(fTopbarHeight, fTopbarHeight)))
		Window::Close();

	ImGui::SetCursorPos(ImVec2(fIconPadding, (fIconPadding / 4) + fTopbarHeight));
	ImGui::Image((void*)(intptr_t)IconTex, vIconSize);

	ImGui::SetCursorPosY(Window::iSizeH - fPadding - vButtonSize.y);

	ImGui::SetCursorPosX(fPadding);
	ImGui::Button("play", vButtonSize);

	ImGui::SameLine();
	ImGui::SetCursorPosX((fPadding * 2) + vButtonSize.x);
	ImGui::Button("mods", vButtonSize);

	ImGui::SameLine();
	ImGui::SetCursorPosX((fPadding * 3) + (vButtonSize.x * 2));
	ImGui::Button("settings", vButtonSize);

	ImGui::SameLine();
	ImGui::SetCursorPosX((fPadding * 4) + (vButtonSize.x * 3));
	ImGui::Button("discord", vButtonSize);

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