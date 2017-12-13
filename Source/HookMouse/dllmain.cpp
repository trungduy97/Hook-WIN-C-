// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Windows.h>
HWND hWndHook;
HHOOK mouse;
HHOOK keyboard;
BOOL Run = false;
HINSTANCE hInstance;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	hInstance = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


LRESULT CALLBACK MouseCallback(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
void SendRDown(POINT p)
{
	HWND hWnd = WindowFromPoint(p);
	SetForegroundWindow(hWnd);
	RECT r;
	if (!GetWindowRect(hWnd, &r))
	{
		return;
	}
	int X = p.x - r.left;
	int Y = p.y - r.top;
	POINT pWin;
	pWin.x = X;
	pWin.y = Y;
	ScreenToClient(hWnd, &pWin);
	PostMessage(hWnd, (int)WM_RBUTTONDOWN, MK_RBUTTON, Y * 65536 + X);
}
void SendRUp(POINT p)
{
	HWND hWnd = WindowFromPoint(p);
	SetForegroundWindow(hWnd);
	RECT r;
	if (!GetWindowRect(hWnd, &r))
	{
		return;
	}
	int X = p.x - r.left;
	int Y = p.y - r.top;
	POINT pWin;
	pWin.x = X;
	pWin.y = Y;
	ScreenToClient(hWnd, &pWin);
	PostMessage(hWnd, (int)WM_RBUTTONUP, 0, Y * 65536 + X);
}

void SendLDown(POINT p)
{
	HWND hWnd = WindowFromPoint(p);
	SetForegroundWindow(hWnd);
	RECT r;
	if (!GetWindowRect(hWnd, &r))
	{
		return;
	}
	int X = p.x - r.left;
	int Y = p.y - r.top;
	POINT pWin;
	pWin.x = X;
	pWin.y = Y;
	ScreenToClient(hWnd, &pWin);
	PostMessage(hWnd, (int)WM_LBUTTONDOWN, MK_LBUTTON, Y * 65536 + X);
}

INT InitMouseHook(HWND hwndYourWindow)
{
	hWndHook = hwndYourWindow;

	if (!(keyboard = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0)))
	{
		MessageBox(hwndYourWindow, L"Failed to install", L"Error", NULL);
		return 0;
	}
	if (!(mouse = SetWindowsHookEx(WH_MOUSE_LL, MouseCallback, NULL, 0)))
	{
		MessageBox(hwndYourWindow, L"Failed to install", L"Error", NULL);
		return 0;
	}
	return 1;
}


void SendLUp(POINT p)
{
	HWND hWnd = WindowFromPoint(p);
	SetForegroundWindow(hWnd);
	RECT r;
	if (!GetWindowRect(hWnd, &r))
	{
		return;
	}
	int X = p.x - r.left;
	int Y = p.y - r.top;
	POINT pWin;
	pWin.x = X;
	pWin.y = Y;
	ScreenToClient(hWnd, &pWin);
	PostMessage(hWnd, (int)WM_LBUTTONUP, 0, Y * 65536 + X);

}


LRESULT CALLBACK MouseCallback(int n, WPARAM wParam, LPARAM lParam)
{
	POINT pos;
	HWND hwndClick = NULL;
	if (n >= 0)
	{
		switch (wParam)
		{
		case WM_RBUTTONUP:
			if (Run == true){
				GetCursorPos(&pos);
				SendLUp(pos);
				return 1;
			}
			break;
		case WM_RBUTTONDOWN:
			if (Run == true) {
				GetCursorPos(&pos);
				SendRDown(pos);
				return 1;
			}
			break;
		case WM_LBUTTONUP:
			if (Run == true) {
				GetCursorPos(&pos);
				SendRUp(pos);
				return 1;
			}
			break;
		case WM_LBUTTONDOWN:{
			if (Run == true){
				GetCursorPos(&pos);
				SendLDown(pos);
				return 1;
			}
			break;
		}
		
		}
	}
	return CallNextHookEx(mouse, n, wParam, lParam);
}
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_KEYDOWN:{
				PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
				if (p->vkCode == VK_F5){
					Run = !Run;
					return 1;
				}
				break;
			}
		}
	}
	return CallNextHookEx(keyboard, nCode, wParam, lParam);
}


INT UninstallMouseHook()
{
	if (mouse != NULL)
	{
		UnhookWindowsHookEx(mouse);
	}
	if (keyboard != NULL)
	{
		UnhookWindowsHookEx(keyboard);
	}
	return 1;
}
