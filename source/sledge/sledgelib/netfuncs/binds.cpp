#include "sledgelib/netfuncs.h"

#include "sledge/misc/binds.h"


/*
	CKeyBind wrapper for C#
*/
struct SKeyBindWrapper {
	CKeyBind* m_Bind;
	void* m_Destroy;
	void* m_UpdateActive;
};

void DestroyBind(SKeyBindWrapper* BindWrapper) {
	delete BindWrapper->m_Bind;
	delete BindWrapper;
}

void UpdateActive(SKeyBindWrapper* BindWrapper, bool bActive) {
	BindWrapper->m_Bind->m_Active = bActive;
}

void* SledgeLib::NetFuncs::CreateBind(unsigned int eType, int iKeyCode, void* pCallback, bool bActive) {
	SKeyBindWrapper* BindWrapper = new SKeyBindWrapper();
	CKeyBind* Bind = new CKeyBind(static_cast<EBindType>(eType), iKeyCode, pCallback, bActive);

	BindWrapper->m_Bind = Bind;
	BindWrapper->m_Destroy = DestroyBind;
	BindWrapper->m_UpdateActive = UpdateActive;

	return BindWrapper;
}