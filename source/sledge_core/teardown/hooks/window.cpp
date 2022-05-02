#include "teardown/hooks.h"

#include "util/log.h"

#include "globals.h"

#include <windef.h>
#include <WinBase.h>
#include <WinUser.h>

#include <detours.h>

typedef HWND(*tCreateWindowExA)	(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
tCreateWindowExA _CreateWindowExA;

HWND hCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
	HWND hWnd = _CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	if (!strcmp(lpWindowName, "Teardown"))
		g_hWnd = hWnd;

	return hWnd;
}

void Teardown::Hooks::Window::Hook() {
	HMODULE hUser32 = GetModuleHandle("USER32.dll");
	if (hUser32 == NULL)
		return;

	_CreateWindowExA = reinterpret_cast<tCreateWindowExA>(GetProcAddress(hUser32, "CreateWindowExA"));

	LogVerbose("CreateWindowExA: {}", reinterpret_cast<void*>(_CreateWindowExA));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_CreateWindowExA, hCreateWindowExA);
	DetourTransactionCommit();
}

void Teardown::Hooks::Window::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_CreateWindowExA, hCreateWindowExA);
	DetourTransactionCommit();
}