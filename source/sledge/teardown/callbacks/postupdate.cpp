#include "teardown/callbacks.h"

#include <vector>
#include <mutex>


std::vector<CCallback*> vPostUpdateCallbacks;
std::mutex PostUpdateMutex;

void _Callbacks::PostUpdate::RegisterCallback(CCallback* Callback) {
	PostUpdateMutex.lock();
	vPostUpdateCallbacks.push_back(Callback);
	PostUpdateMutex.unlock();
}

void _Callbacks::PostUpdate::UnregisterCallback(CCallback* Callback) {
	PostUpdateMutex.lock();
	vPostUpdateCallbacks.erase(std::remove(vPostUpdateCallbacks.begin(), vPostUpdateCallbacks.end(), Callback), vPostUpdateCallbacks.end());
	PostUpdateMutex.unlock();
}

void _Callbacks::OnPostUpdate() {
	for (CCallback* Callback : vPostUpdateCallbacks) {
		if (!Callback->m_Active)
			continue;

		Callback->m_Func();
	}
}