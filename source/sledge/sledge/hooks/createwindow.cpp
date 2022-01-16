#include "sledge/hooks.h"

#include "globals.h"

#include <windef.h>
#include <WinUser.h>
#include <WinBase.h>

#include <processthreadsapi.h>

HHOOK Hook;
LRESULT CBTProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	if (nCode != HCBT_CREATEWND)
		return CallNextHookEx(Hook, nCode, wParam, lParam);

	if (nCode == HCBT_CREATEWND && !g_WindowReady) {
		CBT_CREATEWNDA* CWArguments = reinterpret_cast<CBT_CREATEWNDA*>(lParam);
		if (!CWArguments)
			return CallNextHookEx(Hook, nCode, wParam, lParam);
		
		if (!strcmp(CWArguments->lpcs->lpszName, "Teardown")) {
			g_hWnd = reinterpret_cast<HWND>(wParam);;
			g_WindowWidth = CWArguments->lpcs->cx - 16;
			g_WindowHeight = CWArguments->lpcs->cy - 39;

			Sledge::Hooks::WndProc();

			g_WindowReady = true;
		}
	}
	return 0;
}

void Sledge::Hooks::CW() { Hook = SetWindowsHookExA(WH_CBT, reinterpret_cast<HOOKPROC>(CBTProc), reinterpret_cast<HINSTANCE>(g_hMod), NULL); }