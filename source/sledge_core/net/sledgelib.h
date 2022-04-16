#pragma once

struct SLUnmanagedInterface {
	bool (*Init) ();
	bool (*Shutdown) ();
};

namespace SledgeLib {
	bool Load();

	bool Init();
	void Shutdown();

	inline SLUnmanagedInterface* Interface;
}