#include "sledge/hooks.h"

#include "globals.h"

#include <windef.h>
#include <WinUser.h>

HHOOK Hook;

LRESULT CBTProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	if (nCode == HCBT_CREATEWND) {
		HWND hWnd = reinterpret_cast<HWND>(wParam);
		char* cClassName = new char[512];
		GetClassName(hWnd, cClassName, sizeof(cClassName));
		if (!strcmp(cClassName, "OpenGL")) {
			g_hWnd = hWnd;
			Sledge::Hooks::WndProc();
		}
		delete[] cClassName;
	}
	return CallNextHookEx(Hook, nCode, wParam, lParam);
}

void Sledge::Hooks::CW() { Hook = SetWindowsHookEx(WH_CBT, reinterpret_cast<HOOKPROC>(CBTProc), reinterpret_cast<HINSTANCE>(g_hMod), NULL); }