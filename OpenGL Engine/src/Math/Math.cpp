#include "Math.h"

using namespace Math;

float Math::lerp(float a, float b, float f) {
	return (a * (1.0f - f)) + (b * f);
}