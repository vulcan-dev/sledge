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
	_Callbacks::OnPreUpdate();
	Update(pGame, pDevice);
	_Callbacks::OnPostUpdate();
}

typedef void (*tSpawnPlayer) (void* pPlayer);
tSpawnPlayer SpawnPlayer;

void hSpawnPlayer(void* pPlayer) {
	SpawnPlayer(pPlayer);
	_Callbacks::OnPlayerSpawn();
}

typedef void (*tUpdatePlayer) (void* pPlayer, float fTimeStep);
tUpdatePlayer UpdatePlayer;

void hUpdatePlayer(void* pPlayer, float fTimeStep) {
	_Callbacks::OnPrePlayerUpdate();
	UpdatePlayer(pPlayer, fTimeStep);
	_Callbacks::OnPostPlayerUpdate();
}

void Teardown::Hooks::CallbackHooks() {
	SpawnPlayer = reinterpret_cast<tSpawnPlayer>(Memory::dwFindPattern(Signatures::SpawnPlayer));
	UpdatePlayer = reinterpret_cast<tUpdatePlayer>(Memory::dwFindPattern(Signatures::UpdatePlayer));
	Update = reinterpret_cast<tUpdate>(Memory::dwFindPattern(Signatures::Update));
	
	LogVerbose("SpawnPlayer: {}", reinterpret_cast<void*>(SpawnPlayer));
	LogVerbose("UpdatePlayer: {}", reinterpret_cast<void*>(UpdatePlayer));
	LogVerbose("Update: {}", reinterpret_cast<void*>(Update));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&SpawnPlayer, hSpawnPlayer);
	DetourAttach(&UpdatePlayer, hUpdatePlayer);
	DetourAttach(&Update, hUpdate);
	DetourTransactionCommit();
}