#include "sledge/hooks.h"
#include "util/log.h"
#include "globals.h"

#include <windef.h>
#include <libloaderapi.h>
#include <processthreadsapi.h>
#include <detours.h>

typedef HWND(*tCreateWindowExA)	(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
tCreateWindowExA oCreateWindowExA;

HWND hCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	HWND hWnd = oCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	if (!strcmp(lpWindowName, "Teardown"))
	{
		g_WindowWidth = nWidth; g_WindowHeight = nHeight;
		g_hWnd = hWnd;
		Sledge::Hooks::WndProc();
	}

	return hWnd;
}

void Sledge::Hooks::CW() {
	HMODULE USER32 = GetModuleHandle("USER32.dll");
	if (USER32 == NULL)
		return;

	oCreateWindowExA = (tCreateWindowExA)GetProcAddress(USER32, "CreateWindowExA");

	LogVerbose("CreateWindowExA: {}", reinterpret_cast<void*>(oCreateWindowExA));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oCreateWindowExA, hCreateWindowExA);
	DetourTransactionCommit();
}