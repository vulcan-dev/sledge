#include "teardown/types/small_string.h"
#include "teardown/hooks.h"
#include "teardown/offsets.h"

#include "util/log.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

#include "globals.h"

typedef void (*tWriteToConsole) (small_string* ssLog);
tWriteToConsole _WriteToConsole;

void hWriteToConsole(small_string* ssLog) {
	Log(ELogType::Teardown, ssLog->c_str(), false);
}

void Teardown::Hooks::WriteToConsole::Hook() {
	_WriteToConsole = reinterpret_cast<tWriteToConsole>(g_BaseAddress + g_Offsets["WriteToConsole"]);

	LogVerbose("WriteToConsole: {}", reinterpret_cast<void*>(_WriteToConsole));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_WriteToConsole, hWriteToConsole);
	DetourTransactionCommit();
}

void Teardown::Hooks::WriteToConsole::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_WriteToConsole, hWriteToConsole);
	DetourTransactionCommit();
}