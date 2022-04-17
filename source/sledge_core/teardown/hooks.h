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

	}

	inline void ApplyHooks() {
		Hooks::HookSwitchState();
		Hooks::HookUpdate();
		Hooks::HookUpdatePlayer();
		Hooks::HookChangeLevel();
		Hooks::HookGameCCtor();
	}

	inline void UndoHooks() {
		Hooks::UnhookSwitchState();
		Hooks::UnhookUpdate();
		Hooks::UnhookUpdatePlayer();
		Hooks::UnhookChangeLevel();
		Hooks::UnhookGameCCtor();
	}
}