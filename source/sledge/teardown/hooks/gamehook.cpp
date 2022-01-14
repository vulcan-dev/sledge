#include "teardown/hooks.h"
#include "teardown/signatures.h"

#include "teardown/classes/game.h"
#include "teardown/classes/scene.h"

#include "sledge/loader.h"

#include "util/memory.h"
#include "util/log.h"

#include <mutex>
#include <thread>

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

typedef CGame* (*tGameCCtor) (void*, void*);
tGameCCtor GameCCtor;

std::once_flag fLateInitCalled;

CGame* hGameCCtor(void* pAlloc, void* pMemory) {
	g_Game = GameCCtor(pAlloc, pMemory);
	g_Scene = g_Game->m_Scene;
	
	LogVerbose("g_Game: {}", reinterpret_cast<void*>(g_Game));
	LogVerbose("g_Scene: {}", reinterpret_cast<void*>(g_Scene));

	std::call_once(fLateInitCalled, [] {
		std::thread LateInitThread(Loader::LateInit);
		LateInitThread.detach();
	});

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