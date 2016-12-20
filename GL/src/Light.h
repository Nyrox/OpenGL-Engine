#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

using glm::vec3;

struct Light {
	Light(vec3 position = {}, vec3 ambient = {}, vec3 diffuse = {}, vec3 specular = {}) : position(position), ambient(ambient), diffuse(diffuse), specular(specular) {
	
	}

	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};