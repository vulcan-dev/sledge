#include "teardown/hooks.h"
#include "teardown/offsets.h"

#include "teardown/classes/game.h"
#include "teardown/classes/scene.h"
#include "teardown/classes/player.h"
#include "teardown/classes/renderer.h"

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
	g_Player = g_Game->m_Player;
	g_Renderer = g_Game->m_Renderer;

	/*
	* reduce player collison radius to prevent collisions with arms
	*/
	if (g_VR)
		g_Player->m_CollisionRadius = 0.1f;

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