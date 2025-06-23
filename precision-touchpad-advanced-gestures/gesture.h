#pragma once
#ifndef _GESTURE_H_
#define _GESTURE_H_

#include <vector>
#include <chrono>
#include "touchpad.h"
#include "config.h"
#include <deque>

struct Stroke {
	std::chrono::steady_clock::time_point beginTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point previousReleaseTime = std::chrono::steady_clock::now(); // for debouncing because touchpads and fingers aren't perfect
	std::vector<TouchData> touchData;
};

extern std::deque<Stroke> activeStroke;

void inputTouchPoints(std::vector<TouchData> touchPoints);

#endif