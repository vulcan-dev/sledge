#include "teardown/callbacks.h"

#include <vector>
#include <mutex>

std::vector<CCallback*> vTickCallbacks;
std::mutex TickMutex;

void _Callbacks::Tick::RegisterCallback(CCallback* Callback) {
	TickMutex.lock();
	vTickCallbacks.push_back(Callback);
	TickMutex.unlock();
}

void _Callbacks::Tick::UnregisterCallback(CCallback* Callback) {
	TickMutex.lock();
	vTickCallbacks.erase(std::remove(vTickCallbacks.begin(), vTickCallbacks.end(), Callback), vTickCallbacks.end());
	TickMutex.unlock();
}

/*
	to-do:
		this func gets called twice on level load, fix pls
*/
void _Callbacks::OnTick() {
	for (CCallback* Callback : vTickCallbacks) {
		if (!Callback->m_Active)
			continue;

		reinterpret_cast<tCallback>(Callback->m_Func)();
	}
}