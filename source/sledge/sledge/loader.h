#pragma once

namespace Loader {
	void Init(void* hModule);

	/*
		LateInit:
			At this point, SteamStub is done unpacking, everything is loaded in memory, but nothing is initialized yet.
			(useful for hooking, finding sigs, etc)
	*/
	void LateInit();

	/*
		LateLateInit:
			At this point CGame has been created, and all the classes within it as well (CScene, CEditor, CPlayer, etc).
	*/
	void LateLateInit();
}