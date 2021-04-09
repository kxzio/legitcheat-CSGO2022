#include "hooks.h"
#include "globals.h"
#include "imgui/imgui.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall hooks::hooked_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const auto getButtonHeld = [uMsg, wParam](bool& bButton, int vKey)
	{
		if (wParam != vKey) return;

		if (uMsg == WM_KEYDOWN)
			bButton = true;
		else if (uMsg == WM_KEYUP)
			bButton = false;
	};

	const auto getButtonToggle = [uMsg, wParam](bool& bButton, int vKey)
	{
		if (wParam != vKey) return;

		if (uMsg == WM_KEYUP)
			bButton = !bButton;
	};

	bool lol = false;

	switch (uMsg) {
	case WM_LBUTTONDOWN:
		g->key_pressed[VK_LBUTTON] = true;
		lol = true;
		break;
	case WM_LBUTTONUP:
		g->key_pressed[VK_LBUTTON] = false;
		lol = true;
		break;
	case WM_RBUTTONDOWN:
		g->key_pressed[VK_RBUTTON] = true;
		lol = true;
		break;
	case WM_RBUTTONUP:
		g->key_pressed[VK_RBUTTON] = false;
		lol = true;
		break;
	case WM_MBUTTONDOWN:
		g->key_pressed[VK_MBUTTON] = true;
		lol = true;
		break;
	case WM_MBUTTONUP:
		g->key_pressed[VK_MBUTTON] = false;
		lol = true;
		break;
	case WM_MOUSEMOVE:

		lol = true;
		break;
	case WM_XBUTTONDOWN:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			g->key_pressed[VK_XBUTTON1] = true;
		}
		else if (button == XBUTTON2)
		{
			g->key_pressed[VK_XBUTTON2] = true;
		}
		lol = true;
		break;
	}
	case WM_XBUTTONUP:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			g->key_pressed[VK_XBUTTON1] = false;
		}
		else if (button == XBUTTON2)
		{
			g->key_pressed[VK_XBUTTON2] = false;
		}
		lol = true;
		break;
	}
	case WM_MOUSEWHEEL:
	{
		lol = true;
		break;
	}
	case WM_SYSKEYDOWN:
		g->key_pressed[18] = true;
		break;
	case WM_SYSKEYUP:
		g->key_pressed[18] = false;
		break;
	case WM_KEYDOWN:
		g->key_pressed[wParam] = true;
		lol = true;
		break;
	case WM_KEYUP:
		g->key_pressed[wParam] = false;
		lol = true;
		break;
	default: break;
	}

	for (int i = 0; i < 256; i++) {
		if (g->key_pressed[i]) {
			if (g->key_pressedticks[i] == 0)
				g->key_down[i] = true;
			else
				g->key_down[i] = false;
			g->key_pressedticks[i]++;
		}
		else
			g->key_pressedticks[i] = 0;
	}
	getButtonToggle(g->gui_opened, VK_INSERT);

	if (g->init_d3d && g->gui_opened) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

		if (ImGui::GetIO().WantTextInput)
			return true;
	}

	auto ret = CallWindowProc(g->old_window, hWnd, uMsg, wParam, lParam);

	return ret;
}
