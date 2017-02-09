#pragma once
#include <Core/Texture.h>
#include <glm/glm.hpp>

using glm::vec3;

class Shader;

/*
	Stores all the data neccessary to render a mesh
*/
struct Material {
	Material(float shininess = 64) : shininess(shininess) {
		forward_pass_override = nullptr;
		heightmap = nullptr;
	}

	Shader* forward_pass_override;
	Shader* shadow_pass_override;


	Texture2D* diffuse;
	Texture2D* specular;

	Texture2D* heightmap;

	float shininess;
};