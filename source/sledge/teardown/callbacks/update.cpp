#include "teardown/callbacks.h"

#include <vector>
#include <mutex>

/*
	pre update
*/
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

/*
	post update
*/
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