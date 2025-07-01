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
	cornerSwipeGesture,
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
RECT CursorClipRect ;
HRESULT hr;
auto clipCursor = []() {
	GetCursorPos(&cursorPos);
	CursorClipRect = { cursorPos.x, cursorPos.y, cursorPos.x, cursorPos.y };
	ClipCursor(&CursorClipRect);
	};
bool isNear(double num1, double num2 , double ToLerance) {
	return abs(num1 - num2) < ToLerance;
}

class gestureEngine
{
private:
	std::deque<Stroke>& recentStrokes;

	void LeftSideSwipeGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 1 
			&& isNear(touchData[0].x, touchData[0].maxX,gesturePlacementToLerance))
		{
			action(recentStrokes);
			clipCursor();
		}
		else
		{
			ClipCursor(NULL);
		}
	}
	void RightSideSwipeGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 1 
			&& isNear(touchData[0].x, 0, gesturePlacementToLerance))
		{
			action(recentStrokes);
			clipCursor();
		}
		else
		{
			ClipCursor(NULL);
		}
	}
	void BottomSwipeGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 1 
			&& isNear(touchData[0].y, touchData[0].maxY, gesturePlacementToLerance))
		{
			action(recentStrokes);
			clipCursor();
		}
		else
		{
			ClipCursor(NULL);
		}
	}
	void TopSwipeGesture(std::function<void(std::deque<Stroke>)>action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 1)
		{
			action(recentStrokes);
			clipCursor();
		}
		else
		{
			ClipCursor(NULL);
		}
	}
	void CornerSwipeGesture(std::function<void(std::deque<Stroke>)>action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 1 )
		{
			action(recentStrokes);
			clipCursor();
		}
		else
		{
			ClipCursor(NULL);
		}
	}
	void TwoFingersHoldGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 2)
		{
			action(recentStrokes);
			clipCursor();
		}
		else
		{
			ClipCursor(NULL);
		}
	}
	void TwofingerHoldSideSwipeGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 2)
		{
			action(recentStrokes);
			clipCursor();
		}
		else
		{
			ClipCursor(NULL);
		}
	}
	void ThreeFingerHoldGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 3)
		{
		action(recentStrokes);
		clipCursor();
		}
		else
		{
			ClipCursor(NULL);
		}
	}
	void FiveFingerHoldGesture(std::function<void(std::deque<Stroke>)> action) {
		auto touchData = recentStrokes[0].touchData;
		if (touchData[0].numberOfFingersOnTrackPad == 5)
		{
			action(recentStrokes);
			clipCursor();
		}
		else
		{
			ClipCursor(NULL);
		}
	}
public:
	gestureEngine(std::deque<Stroke>& activeStroke):recentStrokes(activeStroke){};
	void printActiveStrokeData(){
		printf("(%d,%d)\n", recentStrokes[0].touchData[0].x, recentStrokes[0].touchData[0].y);
	}
	
	void ActivateGesture(TouchGestures gesture , std::function<void(std::deque<Stroke>)>action) {
		if(recentStrokes[0].touchData[0].onSurface 
			&& activeStrokes[0].touchData[0].distance > fingerPlacementToLerance){
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
			case cornerSwipeGesture:
				CornerSwipeGesture(action);
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
		else
		{
			ClipCursor(NULL);
		}
	}
};



