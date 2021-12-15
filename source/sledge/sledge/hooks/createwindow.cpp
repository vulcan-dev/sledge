#include "sledge/hooks.h"
#include "sledge/loader.h"

#include "globals.h"

#include <windef.h>
#include <WinBase.h>

#include <detours.h>

#include <mutex>

typedef HWND (*tCreateWindowExA)	(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
tCreateWindowExA CreateWindowExA;

std::once_flag fInitialized;

HWND hCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
	std::call_once(fInitialized, Loader::LateInit);
	/*if (!lstrcmp(lpWindowName, "Teardown")) {
		size_t lCharBuffSize = strlen(g_Build) + 8;
	};*/

	return CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

bool SledgeHooks::CW() {
	HMODULE USER32 = GetModuleHandle("USER32.dll");

	if (USER32 == NULL)
		return false;

	CreateWindowExA = reinterpret_cast<tCreateWindowExA>(GetProcAddress(USER32, "CreateWindowExA"));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&CreateWindowExA, hCreateWindowExA);
	DetourTransactionCommit();

	return true;
}