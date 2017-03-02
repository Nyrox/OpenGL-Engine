#pragma once

#include <gl_core_4_3.hpp>
#include <glm/glm.hpp>

struct PointLight {
	PointLight(glm::vec3 t_position = glm::vec3(), glm::vec3 t_color = glm::vec3(1), float t_intensity = 1.0, float t_ambient = 0.15)
		: position(t_position), color(t_color), intensity(t_intensity), ambientIntensity(t_ambient) {
		
		setEffectiveRange(100);
	}
	
	glm::vec3 position;
	glm::vec3 color;
	float ambientIntensity;
	float intensity;


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