#include "ui/menu.h"
#include "ui/window.h"

#include "util/resources.h"

#include "game/teardown.h"
#include "util/log.h"
#include "tinyjson.h"

#include <thread>
#include <mutex>


#include <imgui_internal.h>

#include <filesystem>
#include <sstream>
#include <fstream>

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

int* ParseColor(std::string name) {
	std::stringstream ss(name);
	std::string item;
	int* arr = new int[3] {255, 255, 255};
	int i = 0;
	while (std::getline(ss, item, ',')) {
		arr[i] = std::stoi(item);
		i++;
	}

	return arr;
}

int* btn_icol;
int* btn_hcol;
int* btn_acol;
int* bg_col;

void ApplyStyle() {
	ImGuiStyle& Style = ImGui::GetStyle();

	std::ifstream config("config.json");
	std::stringstream buffer;
	buffer << config.rdbuf();
	config.close();

	tiny::TinyJson json;
	json.ReadJson(buffer.str());

	std::string sbtn_icol = json.Get<std::string>("button_icol");
	std::string sbtn_hcol = json.Get<std::string>("button_hcol");
	std::string sbtn_acol = json.Get<std::string>("button_acol");
	std::string sbg_col   = json.Get<std::string>("bg_col");

	btn_icol = ParseColor(sbtn_icol);
	btn_hcol = ParseColor(sbtn_hcol);
	btn_acol = ParseColor(sbtn_acol);
	bg_col   = ParseColor(sbg_col);

	Style.Colors[ImGuiCol_Button] = Menu::Colors::FromRGB(btn_icol[0], btn_icol[1], btn_icol[2], 255);
	Style.Colors[ImGuiCol_ButtonHovered] = Menu::Colors::FromRGB(btn_hcol[0], btn_hcol[1], btn_hcol[2], 255);
	Style.Colors[ImGuiCol_ButtonActive] = Menu::Colors::FromRGB(btn_acol[0], btn_acol[1], btn_acol[2], 255);
	Style.Colors[ImGuiCol_WindowBg] = Menu::Colors::FromRGB(bg_col[0], bg_col[1], bg_col[2], 255);
}

void Menu::InitFonts() {
	ImContext = ImGui::GetCurrentContext();
	ImContext->IO.Fonts->AddFontDefault();
	font_l = ImContext->IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16);
	font_s = ImContext->IO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 12);
}

void Menu::Draw() {
	std::call_once(fInitialized, [] {
		ApplyStyle();
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
		WindowDrawList->AddText(ImVec2((Window::iSizeW / 2) - (ImGui::CalcTextSize("Sledge").x / 2) + ImGui::GetFontSize() / 2, 4), Menu::Colors::White, "Sledge");
		ImGui::PopFont();
		ImGui::PushFont(font_s);
		WindowDrawList->AddText(ImVec2((Window::iSizeW / 2) - (ImGui::CalcTextSize(cBuildText).x / 2) + ImGui::GetFontSize() / 2, (fTopBarPadding * 2) + vTitleSize.y), Menu::Colors::White, cBuildText);
		ImGui::PopFont();
		ImGui::PushFont(font_l);

		ImGui::SetCursorPosX(Window::iSizeW - fTopBarHeight);
		if (ImGui::Button("X", ImVec2(fTopBarHeight, fTopBarHeight)))
			Window::Close();

#define GET_VARIABLE_NAME(Variable) (#Variable)

		auto SetColor = [](int* btn) -> void {
			std::ofstream out("config.json", std::ios::out | std::ios::app);
			tiny::TinyJson json;

			std::ifstream in("config.json");
			std::stringstream buffer;
			buffer << in.rdbuf();
			in.close();

			json.ReadJson(buffer.str());
			LogVerbose("JSON: {0}", buffer.str());

			// convert col to string separated by commas
			std::stringstream ss;
			ss << btn[0] << "," << btn[1] << "," << btn[2];

			json[GET_VARIABLE_NAME(btn)].Set(ss.str());
			std::string str = json.WriteJson();
			out << str;

			out.close();
		};

		switch (iCurrentTab) {
		case 0:
			ImGui::SetCursorPos(ImVec2(fIconPadding, (fIconPadding / 4) + fTopBarHeight));
			ImGui::Image((void*)(intptr_t)iIconTexture, vIconSize);
			break;
		case 1: {
			WindowDrawList->AddText(ImVec2((Window::iSizeW / 2) - (ImGui::CalcTextSize("Available Mods").x / 2) + ImGui::GetFontSize() / 2, fTopBarHeight + 12), Menu::Colors::White, "Available Mods");
			int modCount = 0;

			for (auto const& p : std::filesystem::directory_iterator("mods")) {
				if (p.is_directory()) {
					std::stringstream ss;
					modCount += 24;

					std::string filename = p.path().filename().string();
					ss << filename;

					WindowDrawList->AddText(ImVec2((Window::iSizeW / 2) - (ImGui::CalcTextSize(ss.str().c_str()).x / 2) + ImGui::GetFontSize() / 2, fTopBarHeight + 14 + modCount), Menu::Colors::White, ss.str().c_str());
				}
			}

			break;
		} case 2:
			ImGui::SetCursorPosX(17);
			ImGui::Text("Idle Color");
			ImGui::SameLine();
			ImGuiStyle& Style = ImGui::GetStyle();
			ImGui::SetCursorPosX(95);
			if (ImGui::SliderInt3("##btn_icol", btn_icol, 1, 255)) {
				//SetColor(btn_icol);
				//Style.Colors[ImGuiCol_Button] = Menu::Colors::FromRGB(btn_icol[0], btn_icol[1], btn_icol[2]);
			}

			ImGui::SetCursorPosX(10);
			ImGui::Text("Hover Color");
			ImGui::SameLine();
			if (ImGui::SliderInt3("##btn_hcol", btn_hcol, 1, 255)) {
				//SetColor(btn_hcol);
				//Style.Colors[ImGuiCol_ButtonHovered] = Menu::Colors::FromRGB(btn_hcol[0], btn_hcol[1], btn_hcol[2]);
			}

			ImGui::SetCursorPosX(10);
			ImGui::Text("Active Color");
			ImGui::SameLine();
			if (ImGui::SliderInt3("##btn_acol", btn_acol, 1, 255)) {
				//SetColor(btn_acol);
				//Style.Colors[ImGuiCol_ButtonActive] = Menu::Colors::FromRGB(btn_acol[0], btn_acol[1], btn_acol[2]);
			}

			ImGui::SetCursorPosX(10);
			ImGui::Text("Background Color");
			ImGui::SameLine();
			if (ImGui::SliderInt3("##bg_col", bg_col, 1, 255)) {
				//SetColor(bg_col);
				//Style.Colors[ImGuiCol_WindowBg] = Menu::Colors::FromRGB(bg_col[0], bg_col[1], bg_col[2]);
			}
			break;
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