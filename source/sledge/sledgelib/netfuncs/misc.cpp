#include "sledgelib/netfuncs.h"

#include "util/log.h"

void SledgeLib::NetFuncs::WriteLog(int eLogType, char* cMsg) {
	Log(static_cast<ELogType>(eLogType), cMsg);
}