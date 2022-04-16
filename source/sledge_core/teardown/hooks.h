#pragma once

namespace Teardown {
	namespace Hooks {
		void HookSwitchState();
		void UnhookSwitchState();

		void HookUpdate();
		void UnhookUpdate();

		void HookUpdatePlayer();
		void UnhookUpdatePlayer();

	}

	inline void ApplyHooks() {
		Hooks::HookSwitchState();
		Hooks::HookUpdate();
		Hooks::HookUpdatePlayer();
	}

	inline void UndoHooks() {
		Hooks::UnhookSwitchState();
		Hooks::UnhookUpdate();
		Hooks::UnhookUpdatePlayer();
	}
}