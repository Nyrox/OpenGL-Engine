#pragma once

#include <gl_core_4_3.hpp>
#include <glm/glm.hpp>
#include <Math/Math.h>



struct PointLight {
	PointLight() {
		setEffectiveRange(55);
	}
	
	glm::vec3 position;
	glm::vec3 color;

	float effectiveRange;
	float constant, linear, quadratic;

	/*
		Calculates the constant, linear and quadratic components of a light based on range.
		It should be noted that this is a rather costly operation and shouldn't be done too lightly.
	*/
	void setEffectiveRange(float range);
};

struct DirectionalLight {

	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};