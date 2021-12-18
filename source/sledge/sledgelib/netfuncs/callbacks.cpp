#include "sledgelib/netfuncs.h"
#include "teardown/callbacks.h"


/*
	CCallback wrapper for C#
*/
struct SCallbackWrapper {
	CCallback* m_Callback;
	void* m_Destroy;
	void* m_SetActive;
};

void DestroyCallback(SCallbackWrapper* CallbackWrapper) {
	delete CallbackWrapper->m_Callback;
	delete CallbackWrapper;
}

void SetCallbackActive(SCallbackWrapper* CallbackWrapper, bool bActive) {
	CallbackWrapper->m_Callback->m_Active = bActive;
}

void* SledgeLib::NetFuncs::CreateCallback(unsigned int eType, void* pCallback, bool bActive) {
	SCallbackWrapper* CallbackWrapper = new SCallbackWrapper();
	CCallback* Callback = new CCallback(static_cast<ECallbackType>(eType), reinterpret_cast<tCallbackFunction>(pCallback), bActive);

	CallbackWrapper->m_Callback = Callback;
	CallbackWrapper->m_Destroy = DestroyCallback;
	CallbackWrapper->m_SetActive = SetCallbackActive;

	return CallbackWrapper;
}