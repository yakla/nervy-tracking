#pragma once
#ifndef _GESTURE_H_
#define _GESTURE_H_

#include <vector>
#include <chrono>
#include "touchpad.h"
#include "config.h"
#include <deque>

struct Stroke {
	std::chrono::steady_clock::time_point previousReleaseTime = std::chrono::steady_clock::now(); // for debouncing because touchpads and fingers aren't perfect
	std::chrono::steady_clock::time_point gestureCurrentTime;
	std::chrono::steady_clock::time_point gestureBeginingTime;
	std::vector<TouchData> touchData;
};

extern std::deque<Stroke> activeStrokes;

void callStrokeGestures(std::vector<TouchData> touchPoints);

#endif