#include "gesture.h"
#include "touchpad.h"
#include "config.h"
#include "gestureEngine.h"
#include <deque>
# define M_PI 3.14159265358979323846
#define MAX_HISTORY_SIZE 10

std::deque<Stroke> activeStroke = {};
short index = 0;
std::chrono::steady_clock::time_point gestureBeginTime = std::chrono::steady_clock::now();
bool repeat[] = {false,false,false,false,false};
GestureEngine engine;

void inputTouchPoints(std::vector<TouchData> touchData)
{
	int lastTimeStamp = -1;
	if (!activeStroke.empty()) {
		lastTimeStamp = activeStroke[0].touchData[0].timestamp;
	}
	if (touchData[0].timestamp != lastTimeStamp)
	{
	Stroke currentStroke;

	for (int touchId = 0; touchId < config.maxTouchPoints; touchId++) {
		TouchData& singleTouchData = touchData[touchId];

		// time point
		if (singleTouchData.onSurface) {

			if (!repeat[touchId]) {
				// new stroke
				currentStroke.beginTime = std::chrono::steady_clock::now();
				repeat[touchId] = true;
			}
		}
		else {
			currentStroke.endTime = std::chrono::steady_clock::now();
			repeat[touchId] = true;
		}
		currentStroke.touchData.push_back(singleTouchData);


		
		if (activeStroke.size()>0)
		{
				TouchData& currentTouch = currentStroke.touchData[touchId];
				TouchData& previousTouch = activeStroke.front().touchData[touchId];

				// distance and angle
				double deltaX = currentTouch.x - previousTouch.x;
				double deltaY = currentTouch.y - previousTouch.y;
				currentTouch.distance = sqrt(deltaX * deltaX + deltaY * deltaY);
				if (currentTouch.distance > 0.001) {
					currentTouch.angle = atan2(deltaY, deltaX);
				}
				else {
					currentTouch.angle = 0.0;
				}
		}
	}
		activeStroke.push_front(currentStroke);

		if (activeStroke.size() >= MAX_HISTORY_SIZE) {
			activeStroke.pop_back();
		}

		if (activeStroke.size()>1) {
			engine.downSwipeGesture(activeStroke);
		}

	}
}
