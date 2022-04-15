#pragma once

namespace Teardown {
	namespace Hooks {
		void HookSwitchState();
		void UnhookSwitchState();
	}

	inline void ApplyHooks() {
		Hooks::HookSwitchState();
	}

	inline void UndoHooks() {
		Hooks::UnhookSwitchState();
	}
}