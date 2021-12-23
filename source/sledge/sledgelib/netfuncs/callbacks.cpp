#include "sledgelib/netfuncs.h"
#include "teardown/callbacks.h"

void SledgeLib::NetFuncs::RegisterCallback(unsigned int iType, void* pFunc) {
	new CCallback(static_cast<ECallbackType>(iType), reinterpret_cast<tCallbackFunction>(pFunc));
}