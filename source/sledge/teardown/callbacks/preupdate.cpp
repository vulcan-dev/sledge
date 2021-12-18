#include "teardown/callbacks.h"

#include <vector>
#include <mutex>

std::vector<CCallback*> vPreUpdateCallbacks;
std::mutex PreUpdateMutex;

void _Callbacks::PreUpdate::RegisterCallback(CCallback* Callback) {
	PreUpdateMutex.lock();
	vPreUpdateCallbacks.push_back(Callback);
	PreUpdateMutex.unlock();
}

void _Callbacks::PreUpdate::UnregisterCallback(CCallback* Callback) {
	PreUpdateMutex.lock();
	vPreUpdateCallbacks.erase(std::remove(vPreUpdateCallbacks.begin(), vPreUpdateCallbacks.end(), Callback), vPreUpdateCallbacks.end());
	PreUpdateMutex.unlock();
}

void _Callbacks::OnPreUpdate() {
	for (CCallback* Callback : vPreUpdateCallbacks) {
		if (!Callback->m_Active)
			continue;

		Callback->m_Func();
	}
}