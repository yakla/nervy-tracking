#include <Windows.h>
#include <vector>
#include <hidusage.h>
#include <hidpi.h>
#pragma comment(lib, "hid.lib")

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#include "utils.h"
#include "touchpad.h"
#include "devices.h"
#include "gesture.h"
#include "config.h"

std::vector<TouchData> touchPoints(config.maxTouchPoints);

LRESULT CALLBACK mBlockMouseInputHookProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms644986(v=vs.85)
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	else
		return -1;
}

void registerRawInput(HWND hwnd)
{
	// register Windows Precision Touchpad top-level HID collection
	RAWINPUTDEVICE rid;

	rid.usUsagePage = HID_USAGE_PAGE_DIGITIZER;
	rid.usUsage = HID_USAGE_DIGITIZER_TOUCH_PAD;
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = hwnd;

	if (RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)))
	{
		printf(FG_GREEN);
		printf("Successfully register touchpad\n");
		printf(RESET_COLOR);
	}
	else
	{
		printf(FG_RED);
		printf("Failed to register touchpad\n");
		printf(RESET_COLOR);
		getLastError();
	}
}

void handleInputMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// following guide: https://docs.microsoft.com/en-us/windows/win32/inputdev/using-raw-input#performing-a-standard-read-of-raw-input
	// Get the size of RAWINPUT by calling GetRawInputData() with pData = NULL

	bool haveSurface = false;
	for (int i = 0; i < 5; i++) {
		if (touchPoints[i].onSurface)
			haveSurface = true;
	}
	UINT rawInputSize;
	PRAWINPUT rawInputData = NULL;
	hidDeviceInfo deviceInfo;

	getRawInputData((HRAWINPUT)lParam, &rawInputSize, (LPVOID*)(&rawInputData));
	// check if this messsage is for a touchpad
	if (checkInput(rawInputSize, rawInputData, deviceInfo)) {
		TouchData currentTouch;
		// try to read touch data to currentTouch
		if (!readInput(rawInputSize, rawInputData, deviceInfo,touchPoints)) {
			printf("error");
			exit(-1);
		}
	}
	free(rawInputData);
}

LRESULT CALLBACK WndProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		registerRawInput(hwnd);
		break;
	case WM_INPUT:
		handleInputMessage(hwnd, uMsg, wParam, lParam);
		printf("1:(%d,%d),2:(%d,%d) \n", touchPoints[0].x, touchPoints[0].y, touchPoints[1].x, touchPoints[1].y);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

int CALLBACK wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	parseInputDevices();

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(0);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("OverlayWindowClass");
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		printf("RegisterClassEx failed\n");
		getLastError();
		return -1;
	}

	// create a window that will never show for input handling
	HWND hwnd = CreateWindowExW(0, _T("OverlayWindowClass"), _T("Background Touchpad Tool"), 0,
		0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);
	if (!hwnd)
	{
		printf("CreateWindow failed\n");
		getLastError();
		return -1;
	}

	//SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE);
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA | LWA_COLORKEY);



	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

int main()
{
	wWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
	return 0;
};