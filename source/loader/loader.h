#pragma once

#include "window.h"
#include "game.h"

namespace Loader {
	inline void Init() {
		if (!Game::GetTeardownPath())
			return;

		if (!Window::Create())
			return;
	}
}