#include "ui/menu.h"
#include "ui/window.h"

#include "util/resources.h"
#include <Ultralight/Ultralight.h>

#include "game/teardown.h"

#include <thread>
#include <mutex>

#include <imgui_internal.h>

#include <filesystem>
#include <sstream>

float fTopBarHeight;
float fTopBarPadding;
float fIconPadding;
float fPadding;

ImVec2 vZero(0, 0);
ImVec2 vButtonSize;
ImVec2 vBuildTextSize;
ImVec2 vTitleSize;
ImVec2 vIconSize;
ImVec2 vPopupTitleSize;

ImGuiContext* ImContext;
ImFont* font_l, *font_s;

char cBuildText[18];

unsigned int iCurrentTab = 0;
unsigned int iIconTexture;

bool bPopupInQueue = false;
const char* cCurrentErrorTitle;
const char* cCurrentErrorMessage;

std::once_flag fInitialized;

void ApplyStyle() {
	ImGuiStyle& Style = ImGui::GetStyle();
	//ImGuiIO& io = ImGui::GetIO();

	Style.Colors[ImGuiCol_Button] = Menu::Colors::LightBlue;
	Style.Colors[ImGuiCol_ButtonHovered] = Menu::Colors::LighterBlue;
	Style.Colors[ImGuiCol_ButtonActive] = Menu::Colors::DarkBlue;
}

void Menu::Init() {
	ApplyStyle();
	ImContext = ImGui::GetCurrentContext();
	ImContext->IO.Fonts->AddFontDefault();
	font_l = ImContext->IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16);
	font_s = ImContext->IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 12);

	sprintf(cBuildText, "Build %s", __DATE__);

	fPadding = Window::iSizeH / 50.f;
	fTopBarPadding = (vTitleSize.y + vBuildTextSize.y) * 0.1f;
	fTopBarHeight = (vTitleSize.y + vBuildTextSize.y) + (fTopBarPadding * 3.f);
	fIconPadding = fPadding * 15;

	vButtonSize = ImVec2((Window::iSizeW - (fPadding * 5)) / 4.f, Window::iSizeH / 10.f);
	vIconSize = ImVec2(Window::iSizeW - (fIconPadding * 2), Window::iSizeW - (fIconPadding * 2));

	iIconTexture = Resources::TextureFromPNG("ICON_PNG");

	ImGui::SetNextWindowSize(ImVec2(Window::iSizeW, Window::iSizeH));
}

void Menu::Draw() {
	std::call_once(fInitialized, [] {
		vTitleSize = ImGui::CalcTextSize("Sledge");
		vBuildTextSize = ImGui::CalcTextSize(cBuildText);
	});

	ImGui::SetNextWindowPos(vZero);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, vZero);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

	ImGui::Begin("main", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar); {
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		static ImDrawList* WindowDrawList = ImGui::GetWindowDrawList();

		WindowDrawList->AddRectFilled(vZero, ImVec2(Window::iSizeW, fTopBarHeight), Menu::Colors::LightBlue);

		ImGui::PushFont(font_l);
		WindowDrawList->AddText(ImVec2((Window::iSizeW / 2.f) - (vTitleSize.x / 2), fTopBarPadding), Menu::Colors::White, "Sledge");
		ImGui::PopFont();
		ImGui::PushFont(font_s);
		WindowDrawList->AddText(ImVec2((Window::iSizeW / 2.f) - (vBuildTextSize.x / 2), (fTopBarPadding * 2) + vTitleSize.y), Menu::Colors::White, cBuildText);
		ImGui::PopFont();
		ImGui::PushFont(font_l);

		ImGui::SetCursorPosX(Window::iSizeW - fTopBarHeight);
		if (ImGui::Button("X", ImVec2(fTopBarHeight, fTopBarHeight)))
			Window::Close();

		switch (iCurrentTab) {
		case 0:
			ImGui::SetCursorPos(ImVec2(fIconPadding, (fIconPadding / 4) + fTopBarHeight));
			ImGui::Image((void*)(intptr_t)iIconTexture, vIconSize);
			break;
		case 1:
			ImGui::Text("Downloaded mods:");

			for (auto const& p : std::filesystem::directory_iterator("mods")) {
				if (p.is_directory()) {
					std::stringstream ss;

					ss << "\t" << p.path().filename();

					ImGui::Text(ss.str().c_str());
				}
			}

			break;
		}

		ImGui::SetCursorPosY(Window::iSizeH - fPadding - vButtonSize.y);
		ImGui::SetCursorPosX(fPadding);

		if (ImGui::Button("Play", vButtonSize))
			std::thread(Teardown::Launch).detach();

		ImGui::SameLine();
		ImGui::SetCursorPosX((fPadding * 2) + vButtonSize.x);
		if (ImGui::Button("Mods", vButtonSize)) {
			iCurrentTab = 1;
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX((fPadding * 3) + (vButtonSize.x * 2));
		if (ImGui::Button("Settings", vButtonSize)) {
			iCurrentTab = 2;
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX((fPadding * 4) + (vButtonSize.x * 3));
		if (ImGui::Button("Discord", vButtonSize))
			system("start https://www.discord.gg/SAAmJ3VSAS");

		if (bPopupInQueue) {
			bPopupInQueue = false;
			ImGui::OpenPopup("MenuPopup");
		}

		ImGui::PopFont();
		ImGui::End();
	}

	/*
		quick hack to get window dragging working
		( has a ton of delay tho )
	*/
	if (ImContext->MovingWindow) {
		ImVec2 vMouseDrag = ImGui::GetMouseDragDelta();
		Window::MoveWindow(static_cast<int>(vMouseDrag.x), static_cast<int>(vMouseDrag.y));
	}
}
