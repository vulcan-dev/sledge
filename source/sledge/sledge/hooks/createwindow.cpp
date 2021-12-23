#include "sledge/hooks.h"
#include "sledge/loader.h"


#include "globals.h"

#include <windef.h>
#include <WinBase.h>
#include <detours.h>

typedef HWND (*tCreateWindowExA)	(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
tCreateWindowExA CreateWindowExA;


HWND hCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
	if (!lstrcmp(lpWindowName, "Teardown")) {
		Loader::LateInit();
				
		g_hWnd = CreateWindowExA(dwExStyle, lpClassName, "Teardown - Sledge", dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
		return reinterpret_cast<HWND>(g_hWnd);
	}

	return CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

bool Sledge::Hooks::CW() {
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