#include "util/log.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void _WriteLog(int eLogType, char* cMsg) { Log(static_cast<ELogType>(eLogType), cMsg); };