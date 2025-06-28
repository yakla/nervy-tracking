#include "gesture.h"
#include "touchpad.h"
#include "config.h"
#include "gestureEngine.h"
#include <deque>
# define M_PI 3.14159265358979323846
#define MAX_HISTORY_SIZE 10

std::deque<Stroke> activeStrokes = {};

short index = 0;
std::chrono::steady_clock::time_point gestureBeginTime;
bool repeat = false;
bool ForceRecordData = false;
gestureEngine engine(activeStrokes);
void inputTouchPoints(std::vector<TouchData> touchData)
{
	Stroke currentStroke;
	if (touchData[0].onSurface) {

		if (!repeat) {
			// new stroke
			gestureBeginTime = std::chrono::steady_clock::now();
			repeat = true;
		}
	}
	else {
		ForceRecordData = true;
		gestureBeginTime = std::chrono::steady_clock::now();
		repeat = false;
	}

	currentStroke.gestureCurrentTime = std::chrono::steady_clock::now();
	currentStroke.gestureBeginingTime = gestureBeginTime;

	int lastTimeStamp = -1;
	if (!activeStrokes.empty()) {
		lastTimeStamp = activeStrokes[0].touchData[0].timestamp;
	}
	if (touchData[0].timestamp != lastTimeStamp || ForceRecordData)
	{
		ForceRecordData = false;
		for (int touchId = 0; touchId < maxTouchPoints; touchId++) {

			TouchData& singleTouchData = touchData[touchId];
		
			//you need to fix the clock timeings .
			currentStroke.touchData.push_back(singleTouchData);


		
			if (activeStrokes.size()>0)
			{
					TouchData& currentTouch = currentStroke.touchData[touchId];
					TouchData& previousTouch = activeStrokes.front().touchData[touchId];

					// distance and angle
					double deltaX = currentTouch.x - previousTouch.x;
					double deltaY = currentTouch.y - previousTouch.y;
					if (previousTouch.onSurface) {
						currentTouch.distance = sqrt(deltaX * deltaX + deltaY * deltaY);
					}
					else {
						currentTouch.distance = 0;
					}
					if (currentTouch.distance > 0.001) {
						currentTouch.angle = atan2(deltaY, deltaX);
					}
					else {
						currentTouch.angle = 0.0;
					}
			}
		}
			activeStrokes.push_front(currentStroke);

		if (activeStrokes.size() >= MAX_HISTORY_SIZE) {
			activeStrokes.pop_back();
		}

		auto action = [](std::deque<Stroke> activeStrokes) {
			printf("LSSG \n");
		};

		if (activeStrokes.size()>1) {
			engine.ActivateGesture(leftSideSwipeGesture, action);
		}

	}
} 
