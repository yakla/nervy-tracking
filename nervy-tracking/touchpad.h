#pragma once
#ifndef _TOUCHPAD_H_
#define _TOUCHPAD_H_
#define numberOfFingerByteData 7
#define gesturePlacementToLerance 140
#define fingerPlacementToLerance 4
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
	int maxX = 0;
	int maxY = 0;
	short numberOfFingersOnTrackPad;
	int x = 0;
	int y = 0;
	int deltaX = 0;
	int deltaY = 0;
	int velocity = 0;
	double angle = 0;
	double distance = 0;
	bool onSurface = false;
	bool wasUpdatedThisFrame = false;
	double timestamp = 0;
	//TouchEventType eventType = RELEASED;
};

bool checkInput(UINT rawInputSize, PRAWINPUT rawInputData, hidDeviceInfo& deviceInfo);

bool readInput(UINT rawInputSize, PRAWINPUT rawInputData, hidDeviceInfo& deviceInfo, std::vector<TouchData>& touchPoints);

//int saveTouchInput(std::vector<TouchData>& touchPoints, TouchData& newTouch);

#endif