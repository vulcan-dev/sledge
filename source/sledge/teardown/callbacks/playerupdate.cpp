#include "teardown/callbacks.h"

#include <vector>
#include <mutex>

/*
	pre player update
*/
std::vector<CCallback*> vPrePlayerUpdateCallbacks;
std::mutex PlayerPreUpdateMutex;

void _Callbacks::PrePlayerUpdate::RegisterCallback(CCallback* Callback) {
	PlayerPreUpdateMutex.lock();
	vPrePlayerUpdateCallbacks.push_back(Callback);
	PlayerPreUpdateMutex.unlock();
}

void _Callbacks::PrePlayerUpdate::UnregisterCallback(CCallback* Callback) {
	PlayerPreUpdateMutex.lock();
	vPrePlayerUpdateCallbacks.erase(std::remove(vPrePlayerUpdateCallbacks.begin(), vPrePlayerUpdateCallbacks.end(), Callback), vPrePlayerUpdateCallbacks.end());
	PlayerPreUpdateMutex.unlock();
}

void _Callbacks::OnPrePlayerUpdate() {
	for (CCallback* Callback : vPrePlayerUpdateCallbacks) {
		if (!Callback->m_Active)
			continue;

		Callback->m_Func();
	}
}

/*
	post player update
*/
std::vector<CCallback*> vPostPlayerUpdateCallbacks;
std::mutex PlayerPostUpdateMutex;

void _Callbacks::PostPlayerUpdate::RegisterCallback(CCallback* Callback) {
	PlayerPostUpdateMutex.lock();
	vPostPlayerUpdateCallbacks.push_back(Callback);
	PlayerPostUpdateMutex.unlock();
}

void _Callbacks::PostPlayerUpdate::UnregisterCallback(CCallback* Callback) {
	PlayerPostUpdateMutex.lock();
	vPostPlayerUpdateCallbacks.erase(std::remove(vPostPlayerUpdateCallbacks.begin(), vPostPlayerUpdateCallbacks.end(), Callback), vPostPlayerUpdateCallbacks.end());
	PlayerPostUpdateMutex.unlock();
}

void _Callbacks::OnPostPlayerUpdate() {
	for (CCallback* Callback : vPostPlayerUpdateCallbacks) {
		if (!Callback->m_Active)
			continue;

		Callback->m_Func();
	}
}