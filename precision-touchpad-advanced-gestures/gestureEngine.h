#pragma once
#include "gesture.h"
#include "touchpad.h"
#include "config.h"
enum touchGestures {
	sideSwipe,
	twoFingersHold,
	downSwipe
};

class GestureEngine
{
public:
	GestureEngine() {}
	void printActiveStrokeData(std::vector<Stroke>& activeStroke) {
	}
	void downSwipeGesture(std::deque<Stroke> activeStroke){
		if (activeStroke[0].touchData[0].maxY - activeStroke[0].touchData[0].y < 200) {
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
			ip[0].type = INPUT_KEYBOARD;
			ip[0].ki.dwFlags = 0;
			ip[1].type = INPUT_KEYBOARD;
			ip[1].ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(ARRAYSIZE(ip), ip, sizeof(INPUT));
		}
	}
	void twoFingersHoldGesture(std::deque<Stroke> activeStroke) {
		auto pressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(activeStroke[0].gestureCurrentTime - activeStroke[0].gestureBeginingTime);
		if (pressDuration.count() > 3000 && activeStroke[0].touchData[0].numberOfFingersOnTrackPad == 2)
		{
		INPUT ip[4] = {};

		ip[0].type = INPUT_KEYBOARD;
		ip[1].type = INPUT_KEYBOARD;
		ip[2].type = INPUT_KEYBOARD;
		ip[3].type = INPUT_KEYBOARD;

		ip[0].ki.wVk = VK_LWIN; 
		ip[0].ki.dwFlags = 0; 

		ip[1].ki.wVk = VK_DOWN;
		ip[1].ki.dwFlags = 0;

		ip[2].ki.wVk = VK_DOWN;
		ip[2].ki.dwFlags = KEYEVENTF_KEYUP;

		ip[3].ki.wVk = VK_LWIN;
		ip[3].ki.dwFlags = KEYEVENTF_KEYUP;

		SendInput(4, ip, sizeof(INPUT));
		}
	}
};

