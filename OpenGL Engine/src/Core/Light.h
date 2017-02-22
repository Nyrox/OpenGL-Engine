#pragma once

#include <gl_core_4_3.hpp>
#include <glm/glm.hpp>

/* 
	TODO:
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