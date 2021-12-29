#include "sledge/misc/binds.h"

#include "teardown/callbacks.h"

#include "util/log.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void _WriteLog(int eLogType, char* cMsg) { Log(static_cast<ELogType>(eLogType), cMsg); }
sledgelib_func void RegisterCallback(unsigned int iType, void* pFunc) { new CCallback(static_cast<ECallbackType>(iType), reinterpret_cast<tCallbackFunction>(pFunc)); }
sledgelib_func void RegisterInputReader(void* pCallback) { new CKeyBind(EBindType::InputReader, 0, pCallback); }