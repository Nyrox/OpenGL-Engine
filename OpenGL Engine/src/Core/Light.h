#pragma once

#include <gl_core_4_3.hpp>
#include <glm/glm.hpp>

struct PointLight {
	PointLight() {
		constant = 1.0;
		linear = 0.07;
		quadratic = 0.017;
	}
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight {

	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};