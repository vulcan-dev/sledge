#pragma once

#include "imgui.h"

namespace Menu {
	void Draw();
	void InitFonts();

	namespace Colors {
		inline ImColor White		(1.f, 1.f, 1.f);
		inline ImColor Black		(0.f, 0.f, 0.f);
		inline ImColor LighterBlue	(.1f, .1f, 1.f);
		inline ImColor LightBlue	(.1f, .1f, .75f);
		inline ImColor Blue			(.1f, .1f, .5);
		inline ImColor DarkBlue		(.05f, .05f, .25f);

		inline ImColor FromRGB(const float& r, const float& g, const float& b, const float& a = 255) {
			ImColor col;

			col.Value.x = r / 255;
			col.Value.y = g / 255;
			col.Value.z = b / 255;
			col.Value.w = a / 255;

			return col;
		}
    }
}