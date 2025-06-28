#pragma once
#include "gesture.h"
#include "touchpad.h"
#include "config.h"
#include <algorithm>
#include <windows.h>
#include <string>
#include <psapi.h>
#include <iostream>
#include <functional>
#include <vector>
#pragma comment(lib, "Psapi.lib")

enum TouchGestures {
	leftSideSwipeGesture,
	rightSideSwipeGesture,
	bottomSwipeGesture,
	topSwipeGesture,
	twoFingersHoldGesture,
	twofingerHoldSideSwipeGesture,
	threeFingerHoldGesture,
	fiveFingerHoldGesture
};

HWND activeHWND;
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
double windowsScalling;
POINT cursorPos;
RECT windowRect;
HRESULT hr;

bool isNear(double num1, double num2) {
	return abs(num1 - num2) < toLerance;
}

class gestureEngine
{
private:
	std::deque<Stroke>& recentStrokes;

	void LeftSideSwipeGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 1 && isNear(touchData[0].x, touchData[0].maxX))
		{
			action(recentStrokes);
		}
	}
	void RightSideSwipeGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 1)
		{
			action(recentStrokes);
		}
	}
	void BottomSwipeGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 1)
		{
			action(recentStrokes);
		}
	}
	void TopSwipeGesture(std::function<void(std::deque<Stroke>)>action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 1)
		{
			action(recentStrokes);
		}
	}
	void TwoFingersHoldGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 2)
		{
			action(recentStrokes);
		}
	}
	void TwofingerHoldSideSwipeGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 2)
		{
			action(recentStrokes);
		}
	}
	void ThreeFingerHoldGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 3)
		{
		action(recentStrokes);
		}
		
	}
	void FiveFingerHoldGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 5)
		{
			action(recentStrokes);
		}
	}
public:
	gestureEngine(std::deque<Stroke>& activeStroke):recentStrokes(activeStroke){};
	void printActiveStrokeData(){
		printf("(%d,%d)\n", recentStrokes[0].touchData[0].x, recentStrokes[0].touchData[0].y);
	}
	
	void ActivateGesture(TouchGestures gesture , std::function<void(std::deque<Stroke>)>action) {
		switch (gesture) {
		case leftSideSwipeGesture:
			LeftSideSwipeGesture(action);
			break;
		case rightSideSwipeGesture:
			RightSideSwipeGesture(action);
			break;
		case bottomSwipeGesture:
			BottomSwipeGesture(action);
			break;
		case topSwipeGesture:
			TopSwipeGesture(action);
			break;
		case twoFingersHoldGesture:
			TwoFingersHoldGesture(action);
			break;
		case twofingerHoldSideSwipeGesture:
			TwofingerHoldSideSwipeGesture(action);
			break;
		case threeFingerHoldGesture:
			ThreeFingerHoldGesture(action);
			break;
		case fiveFingerHoldGesture:
			FiveFingerHoldGesture(action);
			break;
		}
	}
};



