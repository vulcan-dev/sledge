#pragma once

#include "util/timer.h"

namespace Teardown {
	namespace Hooks {
		void HookSwitchState();
		void UnhookSwitchState();

		void HookUpdate();
		void UnhookUpdate();

		void HookUpdatePlayer();
		void UnhookUpdatePlayer();

		void HookChangeLevel();
		void UnhookChangeLevel();

		void HookGameCCtor();
		void UnhookGameCCtor();

		void HookRegisterLuaFunctions();
		void UnhookRegisterLuaFunctions();
	}

	inline void ApplyHooks() {
		MONITOR();
		Hooks::HookSwitchState();
		Hooks::HookUpdate();
		Hooks::HookUpdatePlayer();
		Hooks::HookChangeLevel();
		Hooks::HookGameCCtor();
		Hooks::HookRegisterLuaFunctions();
	}

	inline void UndoHooks() {
		MONITOR();
		Hooks::UnhookSwitchState();
		Hooks::UnhookUpdate();
		Hooks::UnhookUpdatePlayer();
		Hooks::UnhookChangeLevel();
		Hooks::UnhookGameCCtor();
		Hooks::UnhookRegisterLuaFunctions();
	}
}