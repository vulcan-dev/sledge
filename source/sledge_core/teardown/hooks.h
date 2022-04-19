#pragma once

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
		Hooks::HookSwitchState();
		Hooks::HookUpdate();
		Hooks::HookUpdatePlayer();
		Hooks::HookChangeLevel();
		Hooks::HookGameCCtor();
		Hooks::HookRegisterLuaFunctions();
	}

	inline void UndoHooks() {
		Hooks::UnhookSwitchState();
		Hooks::UnhookUpdate();
		Hooks::UnhookUpdatePlayer();
		Hooks::UnhookChangeLevel();
		Hooks::UnhookGameCCtor();
		Hooks::UnhookRegisterLuaFunctions();
	}
}