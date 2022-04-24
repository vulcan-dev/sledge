#include "teardown/hooks.h"
#include "teardown/offsets.h"

#include "teardown/classes/game.h"
#include "teardown/classes/scene.h"

#include "sledge/loader.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

#include "globals.h"

typedef Game* (*tGameCCtor) (void* pMemory, void* pDevice);
tGameCCtor GameCCtor;

Game* hGameCCtor(void* pMemory, void* pDevice) {
	g_Game = GameCCtor(pMemory, pDevice);
	g_Scene = g_Game->m_Scene;

	LogVerbose("g_Game: {}", reinterpret_cast<void*>(g_Game));
	CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Loader::GameInit), NULL, NULL, NULL);
	return g_Game;
}

void Teardown::Hooks::GameConstructor::Hook() {
	GameCCtor = reinterpret_cast<tGameCCtor>(g_BaseAddress + g_Offsets["Game::Game"]);

	LogVerbose("Game::Game: {}", reinterpret_cast<void*>(GameCCtor));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&GameCCtor, hGameCCtor);
	DetourTransactionCommit();
}

void Teardown::Hooks::GameConstructor::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&GameCCtor, hGameCCtor);
	DetourTransactionCommit();
}