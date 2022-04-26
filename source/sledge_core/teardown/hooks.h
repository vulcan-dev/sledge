#pragma once

#include "util/timer.h"

namespace Teardown {
	namespace Hooks {
		namespace Window {
			void Hook();
			void Unhook();
		}

		namespace SwitchState {
			void Hook();
			void Unhook();
		}

		namespace Update {
			void Hook();
			void Unhook();
		}

		namespace UpdatePlayer {
			void Hook();
			void Unhook();
		}

		namespace LevelChange {
			void Hook();
			void Unhook();
		}

		namespace GameConstructor {
			void Hook();
			void Unhook();
		}

		namespace RegisterLuaFunctions {
			void Hook();
			void Unhook();
		}

		namespace WriteToConsole {
			void Hook();
			void Unhook();
		}

		namespace IsWindowForegroundWindow {
			void Hook();
			void Unhook();
		}

		namespace SetCursorCaptured	{
			void Hook();
			void Unhook();
		}
	}

	inline void ApplyHooks() {
		MONITOR();
		Hooks::Window::Hook();
		Hooks::SwitchState::Hook();
		Hooks::Update::Hook();
		Hooks::UpdatePlayer::Hook();
		Hooks::LevelChange::Hook();
		Hooks::GameConstructor::Hook();
		Hooks::RegisterLuaFunctions::Hook();
		Hooks::WriteToConsole::Hook();
		Hooks::IsWindowForegroundWindow::Hook();
		Hooks::SetCursorCaptured::Hook();
	}

	inline void UndoHooks() {
		MONITOR();
		Hooks::Window::Unhook();
		Hooks::SwitchState::Unhook();
		Hooks::Update::Unhook();
		Hooks::UpdatePlayer::Unhook();
		Hooks::LevelChange::Unhook();
		Hooks::GameConstructor::Unhook();
		Hooks::RegisterLuaFunctions::Unhook();
		Hooks::WriteToConsole::Unhook();
		Hooks::IsWindowForegroundWindow::Unhook();
		Hooks::SetCursorCaptured::Unhook();
	}
}