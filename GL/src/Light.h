#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

struct PointLight {
	PointLight() {
		constant = 1.0;
		linear = 0.07;
		quadratic = 0.017;
	}
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};