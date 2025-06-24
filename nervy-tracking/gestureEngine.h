#pragma once
#include "gesture.h"
#include "touchpad.h"
#include "config.h"
#include "winuser.h"
#include <algorithm>
#include <windows.h>
#include <string>
#include <psapi.h>
#include <ShellScalingApi.h>
#include <iostream>
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Shcore.lib")


enum touchGestures {
	sideSwipe,
	twoFingersHold,
	downSwipe
};

enum class DragState {
	Idle,      
	Ready,     
	Dragging  
};
HWND activeHWND;
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
bool toMaximize = true;
bool HoldOnce = false;
bool moved = false;
double windowsScalling;
POINT cursorPos;
RECT windowRect;
HRESULT hr;
class GestureEngine
{
public:
	GestureEngine() {}
	void printActiveStrokeData(std::vector<Stroke>& activeStroke) {
	}
	void downSwipeGesture(std::deque<Stroke> activeStroke){
		if (activeStroke[0].touchData[0].maxY - activeStroke[0].touchData[0].y < 200 && activeStroke[0].touchData[0].onSurface) {
			INPUT ip[2] = {};
			if (activeStroke[1].touchData[0].x - activeStroke[0].touchData[0].x > toLerance && activeStroke[1].touchData[0].x - activeStroke[0].touchData[0].x > 0)
			{
				ip[0].ki.wVk = VK_LEFT;
				ip[1].ki.wVk = VK_LEFT;
			}
			else if (activeStroke[1].touchData[0].x - activeStroke[0].touchData[0].x < -toLerance && activeStroke[1].touchData[0].x - activeStroke[0].touchData[0].x < 0)
			{
				ip[0].ki.wVk = VK_RIGHT;
				ip[1].ki.wVk = VK_RIGHT;
			}
			{

			}
			POINT cursorPos;
			GetCursorPos(&cursorPos);

			RECT lockRect;
			lockRect.left = cursorPos.x;
			lockRect.top = cursorPos.y;
			lockRect.right = cursorPos.x;
			lockRect.bottom = cursorPos.y;
			ClipCursor(&lockRect);

			ip[0].type = INPUT_KEYBOARD;
			ip[0].ki.dwFlags = 0;
			ip[1].type = INPUT_KEYBOARD;
			ip[1].ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(ARRAYSIZE(ip), ip, sizeof(INPUT));
		}
		else
		{
			ClipCursor(NULL);
		}
	}
	void twoFingersHoldGesture(std::deque<Stroke> activeStroke) {
		auto pressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(activeStroke[0].gestureCurrentTime - activeStroke[0].gestureBeginingTime);
		INPUT ip[1] = {};
		ip[0].type = INPUT_MOUSE;
		if (!activeStroke[0].touchData[0].onSurface)
		{
			moved = false;
		}
		if (pressDuration.count() > 1000 && activeStroke[0].touchData[0].numberOfFingersOnTrackPad == 2 && !moved) {
			GetCursorPos(&cursorPos);
			if (activeHWND == NULL)
			{
				activeHWND = WindowFromPoint(cursorPos);
			}
			if (toMaximize) {
				ShowWindow(activeHWND, SW_MAXIMIZE);
				toMaximize = false;
			}
			windowsScalling = getWindowScalling();
			//std::cout << windowsScalling;
			
			if (HoldOnce) {
				SetCursorPos(static_cast<int>(windowsScalling * static_cast<double>(activeStroke[0].touchData[0].x * screenWidth / activeStroke[0].touchData[0].maxX)), static_cast<int>(windowsScalling * static_cast<double>(activeStroke[0].touchData[0].y * screenHeight / activeStroke[0].touchData[0].maxY))-60);
			}
			if (!HoldOnce)
			{
				ip[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
				SetCursorPos(windowsScalling * screenWidth / 2, 0);
				SendInput(ARRAYSIZE(ip), ip, sizeof(INPUT));
				HoldOnce = true;
			}
		}
		else
		{
			if (HoldOnce) {
				ip[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
				SendInput(ARRAYSIZE(ip), ip, sizeof(INPUT));
			}
			if (activeStroke[0].touchData[0].distance > toLerance) {
				moved = true;
			}
			toMaximize = true;
			HoldOnce = false;
			activeHWND = NULL;
		}
		
	}
	double getWindowScalling() {
		GetCursorPos(&cursorPos);

		HMONITOR hMonitor = MonitorFromPoint(cursorPos, MONITOR_DEFAULTTONEAREST);

		UINT dpiX, dpiY;
		hr = GetDpiForMonitor(
			hMonitor,                  
			MDT_EFFECTIVE_DPI,         
			&dpiX,                     
			&dpiY                      
		);
		return static_cast<double>(dpiX) / 96.0;
	}
};

