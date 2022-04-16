#pragma once

struct SLCallbackInterface {
	void (*StateChange) (unsigned int);
	void (*LevelLoad) (const char*);
	void (*PreUpdate) ();
	void (*PostUpdate) ();
	void (*PrePlayerUpdate) ();
	void (*PostPlayerUpdate) ();
};

struct SLUnmanagedInterface {
	bool (*Init) ();
	bool (*Shutdown) ();
	SLCallbackInterface (*GetCallbackInterface) ();
};

namespace SledgeLib {
	bool Load();

	bool Init();
	void Shutdown();

	inline SLUnmanagedInterface* Interface;
	inline SLCallbackInterface* CallbackInterface;
}