#pragma once
#include <Texture.h>
#include <glm/glm.hpp>

using glm::vec3;

class Shader;

struct Material {
	Material(float shininess = 64) : shininess(shininess) {
		forward_pass_override = nullptr;
		heightmap = nullptr;
	}

	Shader* forward_pass_override;
	Shader* shadow_pass_override;


	Texture diffuse;
	Texture specular;

	Texture* heightmap;

	float shininess;
};