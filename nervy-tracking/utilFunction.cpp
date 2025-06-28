#include <stdlib.h>
#include "touchpad.h"
bool isNear(double num1, double num2) {
	return abs(num1 - num2) < toLerance;
}