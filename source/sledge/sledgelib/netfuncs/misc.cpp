#include "sledge/misc/binds.h"

#include "teardown/callbacks.h"

#include "util/log.h"

#define _sledgelib_export extern "C" __declspec(dllexport)

_sledgelib_export void WriteLog(int eLogType, char* cMsg) { Log(static_cast<ELogType>(eLogType), cMsg); }
_sledgelib_export void RegisterCallback(unsigned int iType, void* pFunc) { new CCallback(static_cast<ECallbackType>(iType), reinterpret_cast<tCallbackFunction>(pFunc)); }
_sledgelib_export void RegisterInputReader(void* pCallback) { new CKeyBind(EBindType::InputReader, 0, pCallback); }