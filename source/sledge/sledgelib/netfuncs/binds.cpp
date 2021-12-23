#include "sledgelib/netfuncs.h"
#include "sledge/misc/binds.h"

void SledgeLib::NetFuncs::RegisterInputReader(void* pCallback) {
	new CKeyBind(EBindType::InputReader, 0, pCallback);
}