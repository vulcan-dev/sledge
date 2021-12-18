#include "teardown/hooks.h"
#include "teardown/signatures.h"
#include "teardown/callbacks.h"

#include <teardown/classes/game.h>

#include "util/memory.h"
#include "util/log.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>


typedef void (*tUpdate)	(CGame* pGame, void* pDevice);
tUpdate Update;
void hUpdate(CGame* pGame, void* pDevice) {
	if (pGame->m_State == EGameState::Playing) {
		_Callbacks::OnPreUpdate();
		Update(pGame, pDevice);
		_Callbacks::OnPostUpdate();
		return;
	}
	Update(pGame, pDevice);
}

typedef void (*tSpawnPlayer) (void* pPlayer);
tSpawnPlayer SpawnPlayer;

void hSpawnPlayer(void* pPlayer) {
	SpawnPlayer(pPlayer);
	_Callbacks::OnPlayerSpawn();
}

void Teardown::Hooks::CallbackHooks() {
	SpawnPlayer = reinterpret_cast<tSpawnPlayer>(Memory::dwFindPattern(Signatures::SpawnPlayer));
	Update = reinterpret_cast<tUpdate>(Memory::dwFindPattern(Signatures::Update));
	
	LogVerbose("SpawnPlayer: {}", reinterpret_cast<void*>(SpawnPlayer));
	LogVerbose("Update: {}", reinterpret_cast<void*>(Update));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&SpawnPlayer, hSpawnPlayer);
	DetourAttach(&Update, hUpdate);
	DetourTransactionCommit();
}