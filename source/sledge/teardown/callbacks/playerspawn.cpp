#include "teardown/callbacks.h"

#include <vector>
#include <mutex>


std::vector<CCallback*> vPlayerSpawnCallbacks;
std::mutex PlayerSpawnMutex;

void _Callbacks::PlayerSpawn::RegisterCallback(CCallback* Callback) {
	PlayerSpawnMutex.lock();
	vPlayerSpawnCallbacks.push_back(Callback);
	PlayerSpawnMutex.unlock();
}

void _Callbacks::PlayerSpawn::UnregisterCallback(CCallback* Callback) {
	PlayerSpawnMutex.lock();
	vPlayerSpawnCallbacks.erase(std::remove(vPlayerSpawnCallbacks.begin(), vPlayerSpawnCallbacks.end(), Callback), vPlayerSpawnCallbacks.end());
	PlayerSpawnMutex.unlock();
}

/*
	to-do:
		this func gets called twice on level load, fix pls
*/
void _Callbacks::OnPlayerSpawn() {
	for (CCallback* Callback : vPlayerSpawnCallbacks) {
		if (!Callback->m_Active)
			continue;

		Callback->m_Func();
	}
}