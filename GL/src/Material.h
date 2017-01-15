#pragma once
#include <Texture.h>
#include <glm/glm.hpp>

using glm::vec3;

struct Material {
	Material(float shininess = 64) : shininess(shininess) {

	}

	Texture diffuse;
	Texture specular;
	float shininess;
};