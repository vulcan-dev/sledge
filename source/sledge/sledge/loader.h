#pragma once
namespace Loader {
	/*
	Init:
		Called the moment the DLL is injected
	*/
	void Init(void* hModule);

	/*
		LateLateInit:
			At this point CGame has been created, and all the classes within it as well (CScene, CEditor, CPlayer, etc).
	*/
	void LateInit();
}