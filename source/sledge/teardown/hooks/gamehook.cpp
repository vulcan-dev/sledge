#include "teardown/hooks.h"
#include "teardown/signatures.h"
#include "teardown/classes/game.h"

#include "sledge/loader.h"

#include "util/memory.h"
#include "util/log.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

typedef CGame* (*tGameCCtor) (void*, void*);
tGameCCtor GameCCtor;

CGame* hGameCCtor(void* pAlloc, void* pMemory) {
	g_Game = GameCCtor(pAlloc, pMemory);
	LogVerbose("g_Game: {}", reinterpret_cast<void*>(g_Game));

	Loader::LateLateInit();

	return g_Game;
}

void Teardown::Hooks::Game() {
	GameCCtor = reinterpret_cast<tGameCCtor>(Memory::dwFindPattern(Signatures::Game));
	LogVerbose("CGame::CGame: {}", reinterpret_cast<void*>(GameCCtor));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&GameCCtor, hGameCCtor);
	DetourTransactionCommit();
}