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

	Style.Colors[ImGuiCol_Button] = Menu::Colors::FromRGB(255, 118, 117);
	Style.Colors[ImGuiCol_ButtonHovered] = Menu::Colors::FromRGB(255, 138, 137);
	Style.Colors[ImGuiCol_ButtonActive] = Menu::Colors::FromRGB(255, 88, 87);
	Style.Colors[ImGuiCol_WindowBg] = Menu::Colors::FromRGB(45, 52, 54);
}

void Menu::InitFonts() {
	ApplyStyle();
	ImContext = ImGui::GetCurrentContext();
	ImContext->IO.Fonts->AddFontDefault();
	font_l = ImContext->IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16);
	font_s = ImContext->IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 12);
}

void Menu::Draw() {
	std::call_once(fInitialized, [] {
		sprintf(cBuildText, "Build %s", __DATE__);

		iIconTexture = Resources::TextureFromPNG("ICON_PNG");

		ImGui::SetNextWindowSize(ImVec2(Window::iSizeW, Window::iSizeH));

		vTitleSize = ImGui::CalcTextSize("Sledge");
		vBuildTextSize = ImGui::CalcTextSize(cBuildText);

		fPadding = Window::iSizeH / 50.f;
		fTopBarPadding = (vTitleSize.y + vBuildTextSize.y) * 0.1f;
		fTopBarHeight = (vTitleSize.y + vBuildTextSize.y) + (fTopBarPadding * 3.f);
		fIconPadding = fPadding * 15;

		//vButtonSize = ImVec2((Window::iSizeW - (fPadding * 5)) / 4.f, Window::iSizeH / 10.f);
		vButtonSize = ImVec2(100, fTopBarHeight);
		vIconSize = ImVec2(Window::iSizeW - (fIconPadding * 2), Window::iSizeW - (fIconPadding * 2));
	});

	ImGui::SetNextWindowPos(vZero);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, vZero);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

	ImGui::Begin("main", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar); {
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		static ImDrawList* WindowDrawList = ImGui::GetWindowDrawList();

		WindowDrawList->AddRectFilled(vZero, ImVec2(Window::iSizeW, fTopBarHeight), Menu::Colors::FromRGB(255, 118, 117));

		ImGui::PushFont(font_l);
		WindowDrawList->AddText(ImVec2((Window::iSizeW / 2) - (vTitleSize.x / 2), fTopBarPadding), Menu::Colors::White, "Sledge");
		ImGui::PopFont();
		ImGui::PushFont(font_s);
		WindowDrawList->AddText(ImVec2((Window::iSizeW / 2) - (vBuildTextSize.x / 2) + ImGui::GetFontSize(), (fTopBarPadding * 2) + vTitleSize.y), Menu::Colors::White, cBuildText);
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
		case 1: {
			WindowDrawList->AddText(ImVec2((Window::iSizeW / 2) - (vBuildTextSize.x / 2) + ImGui::GetFontSize(), fTopBarHeight + 12), Menu::Colors::White, "Available Mods");
			//ImGui::Text("Downloaded mods:");
			int mc = 0;

			for (auto const& p : std::filesystem::directory_iterator("mods")) {
				if (p.is_directory()) {
					std::stringstream ss;
					mc += 24;

					std::string filename = p.path().filename().string();
					filename.erase(0, 0);
					filename.erase(filename.size() - 1);
					ss << filename;

					WindowDrawList->AddText(ImVec2((Window::iSizeW / 2) - (ImGui::CalcTextSize(ss.str().c_str()).x / 2), fTopBarHeight + 14 + mc), Menu::Colors::White, ss.str().c_str());
				}
			}

			break;
		} case 2:
			ImGui::Text("WIP");
		}

		WindowDrawList->AddRectFilled(ImVec2(0, Window::iSizeH - fTopBarHeight), ImVec2(Window::iSizeW, Window::iSizeH), Menu::Colors::FromRGB(255, 118, 117));
		ImGui::SetCursorPosX(20);

		ImGui::SetCursorPosY(Window::iSizeH - fTopBarHeight);
		if (ImGui::Button("Play", vButtonSize))
			std::thread(Teardown::Launch).detach();

		ImGui::SameLine();
		ImGui::SetCursorPosX((20 * 2) + vButtonSize.x);
		if (ImGui::Button("Mods", vButtonSize)) {
			iCurrentTab = 1;
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX((20 * 3) + (vButtonSize.x * 2));
		if (ImGui::Button("Settings", vButtonSize)) {
			iCurrentTab = 2;
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX((20 * 4) + (vButtonSize.x * 3));
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