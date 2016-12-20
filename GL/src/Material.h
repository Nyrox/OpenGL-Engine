#pragma once
#include <glm/glm.hpp>

using glm::vec3;

struct Material {
	Material() {};
	// Utility constructor which will set all modifiers to __color.
	// Useful if you just want a monotone material up fast
	Material(glm::vec3 __color, float shininess = 32) : ambient(__color), diffuse(__color), specular(__color), shininess(shininess) {

	}

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};