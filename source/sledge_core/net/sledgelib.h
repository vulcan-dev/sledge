#pragma once

#pragma pack(push, 1)
struct SLCallbackInterface {
	void (*StateChange) (unsigned int);
	void (*LevelLoad) (const char*);
	void (*PreUpdate) ();
	void (*PostUpdate) ();
	void (*PrePlayerUpdate) (float);
	void (*PostPlayerUpdate) (float);
};
#pragma pack(pop)

struct SLUnmanagedInterface {
	bool (*Init) ();
	bool (*Shutdown) ();
	SLCallbackInterface (*GetCallbackInterface) ();
	void* (*AllocateString) (size_t lLength);
	int (*LuaFunctionWrapper) (class ScriptCore* pSC, class lua_State* L, char* cFunctionName);
};

namespace SledgeLib {
	bool Load();

	bool Init();
	void Shutdown();

	inline SLUnmanagedInterface* Interface;
	inline SLCallbackInterface* CallbackInterface;
}