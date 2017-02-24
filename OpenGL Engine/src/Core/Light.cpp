#include "Light.h"

/*
Range Constant Linear Quadratic
3250, 1.0, 0.0014, 0.000007
600, 1.0, 0.007, 0.0002
325, 1.0, 0.014, 0.0007
200, 1.0, 0.022, 0.0019
160, 1.0, 0.027, 0.0028
100, 1.0, 0.045, 0.0075
65, 1.0, 0.07, 0.017
50, 1.0, 0.09, 0.032
32, 1.0, 0.14, 0.07
20, 1.0, 0.22, 0.20
13, 1.0, 0.35, 0.44
7, 1.0, 0.7, 1.8
*/


void PointLight::setEffectiveRange(float range) {
	// We only store the linear and quadratic components, since constant is always gonna be 1
	static Curve<std::pair<float, float>> lightValues = {
		std::make_pair(7,	std::make_pair(0.7, 1.8)),
		std::make_pair(13,	std::make_pair(0.35, 0.44)),
		std::make_pair(20,	std::make_pair(0.22, 0.20)),
		std::make_pair(32,	std::make_pair(0.14, 0.07)),
		std::make_pair(50,	std::make_pair(0.09, 0.032)),
		std::make_pair(65,	std::make_pair(0.07, 0.017)),
		std::make_pair(100, std::make_pair(0.045, 0.0075)),
		std::make_pair(160, std::make_pair(0.027, 0.0028)),
		std::make_pair(200,	std::make_pair(0.022, 0.0019)),
		std::make_pair(325, std::make_pair(0.014, 0.0007)),
		std::make_pair(600, std::make_pair(0.007, 0.0002))
	};

	std::pair<float, float> val = lightValues.samplePoint(range, [](std::pair<float, float> v1, std::pair<float, float> v2, float l) {
		return std::make_pair(Math::lerp(std::get<0>(v1), std::get<0>(v2), l), Math::lerp(std::get<1>(v1), std::get<1>(v2), l));
	});
	this->linear = std::get<0>(val);
	this->quadratic = std::get<1>(val);
	this->constant = 1;
}