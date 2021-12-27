#include "teardown/hooks.h"
#include "teardown/signatures.h"
#include "teardown/small_string.h"

#include "util/log.h"
#include "util/memory.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

void hLog(small_string* ssLog) {
	Log(ELogType::Teardown, ssLog->c_str(), false);
}

void Teardown::Hooks::Log() {
	DWORD64 dwLog = Memory::dwFindPattern(Signatures::Log);

	LogVerbose("Log: {}", dwLog);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)dwLog, hLog);
	DetourTransactionCommit();
}