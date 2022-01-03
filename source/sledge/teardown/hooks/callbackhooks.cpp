#include "teardown/hooks.h"
#include "teardown/signatures.h"
#include "teardown/callbacks.h"

#include <teardown/classes/game.h>

#include "util/memory.h"
#include "util/log.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

typedef void (*tSetState) (CGame* pGame, unsigned int iStatus);
tSetState SetState;
void hSetState(CGame* pGame, unsigned int iState) {
	SetState(pGame, iState);
	_Callbacks::OnStateChange(iState);
}

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

typedef void (*tTick) (void* pScene, void* a2, void* a3);
tTick Tick;

void hTick(void* pScene, void* a2, void* a3) {
	Tick(pScene, a2, a3);
	_Callbacks::OnTick();
}

void Teardown::Hooks::CallbackHooks() {
	SpawnPlayer = reinterpret_cast<tSpawnPlayer>(Memory::dwFindPattern(Signatures::SpawnPlayer));
	UpdatePlayer = reinterpret_cast<tUpdatePlayer>(Memory::dwFindPattern(Signatures::UpdatePlayer));
	Update = reinterpret_cast<tUpdate>(Memory::dwFindPattern(Signatures::Update));
	SetState = reinterpret_cast<tSetState>(Memory::dwFindPattern(Signatures::SetState));
	Tick = reinterpret_cast<tTick>(Memory::dwFindPattern(Signatures::Tick));
	
	LogVerbose("SpawnPlayer: {}", reinterpret_cast<void*>(SpawnPlayer));
	LogVerbose("UpdatePlayer: {}", reinterpret_cast<void*>(UpdatePlayer));
	LogVerbose("UpdatePlayer: {}", reinterpret_cast<void*>(UpdatePlayer));
	LogVerbose("Update: {}", reinterpret_cast<void*>(Update));
	LogVerbose("Tick: {}", reinterpret_cast<void*>(Tick));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&SpawnPlayer, hSpawnPlayer);
	DetourAttach(&UpdatePlayer, hUpdatePlayer);
	DetourAttach(&Update, hUpdate);
	DetourAttach(&SetState, hSetState);
	DetourAttach(&Tick, hTick);
	DetourTransactionCommit();
}