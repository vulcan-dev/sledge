#include "sledge/hooks.h"
#include "sledge/misc/binds.h"

#include "globals.h"

#include <windef.h>
#include <WinUser.h>

WNDPROC oWndProc;
LRESULT APIENTRY hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_KEYUP:
		CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam); // pass keyup events to the game, so keys don't get "stuck" (very lazy fix, i'll write something better later)
		break;
	}

	if (GetForegroundWindow() != g_hWnd)
		return true;

	Binds::OnInput(uMsg, wParam);

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void Sledge::Hooks::Wnd() {
	g_hWnd = FindWindow(0, "Teardown");

	oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(reinterpret_cast<HWND>(g_hWnd), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hWndProc))); 
}