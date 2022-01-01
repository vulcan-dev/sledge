#include "teardown/callbacks.h"

#include <vector>
#include <mutex>

typedef void (*tStateChangeCallback) (unsigned int iState);

std::vector<CCallback*> vStateChangeCallbacks;
std::mutex StateChangeMutex;

void _Callbacks::StateChange::RegisterCallback(CCallback* Callback) {
	StateChangeMutex.lock();
	vStateChangeCallbacks.push_back(Callback);
	StateChangeMutex.unlock();
}

void _Callbacks::StateChange::UnregisterCallback(CCallback* Callback) {
	StateChangeMutex.lock();
	vStateChangeCallbacks.erase(std::remove(vStateChangeCallbacks.begin(), vStateChangeCallbacks.end(), Callback), vStateChangeCallbacks.end());
	StateChangeMutex.unlock();
}

/*
	to-do:
		this func gets called twice on level load, fix pls
*/
void _Callbacks::OnStateChange(unsigned int iState) {
	for (CCallback* Callback : vStateChangeCallbacks) {
		if (!Callback->m_Active)
			continue;
		
		reinterpret_cast<tStateChangeCallback>(Callback->m_Func)(iState);
	}
}