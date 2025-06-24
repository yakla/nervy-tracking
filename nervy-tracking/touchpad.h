#pragma once
#ifndef _TOUCHPAD_H_
#define _TOUCHPAD_H_
#define numberOfFingerByteData 7
#define toLerance 10
#include "utils.h"
#include <Windows.h>

enum TouchEventType {
	RELEASED,
	TOUCH_DOWN,
	TOUCH_MOVE,
	TOUCH_UP,
	TOUCH_MOVE_UNCHANGED
};

struct TouchData
{
	short touchID = -1;
	long maxX = 0;
	long maxY = 0;
	short numberOfFingersOnTrackPad;
	long x = 0;
	long y = 0;
	short angle = 0;
	long distance = 0;
	bool onSurface = false;
	bool wasUpdatedThisFrame = false;
	short timestamp = 0;
	TouchEventType eventType = RELEASED;
};

bool checkInput(UINT rawInputSize, PRAWINPUT rawInputData, hidDeviceInfo& deviceInfo);

bool readInput(UINT rawInputSize, PRAWINPUT rawInputData, hidDeviceInfo& deviceInfo, std::vector<TouchData>& touchPoints);

//int saveTouchInput(std::vector<TouchData>& touchPoints, TouchData& newTouch);

#endif