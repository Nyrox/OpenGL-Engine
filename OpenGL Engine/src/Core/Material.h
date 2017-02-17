#pragma once
#include <Core/Texture.h>
#include <glm/glm.hpp>
#include <variant>

using glm::vec3;

class Shader;

/*
	Stores all the data neccessary to render a mesh
*/
struct Material {
	Material(float shininess = 64) : shininess(shininess) {
		forward_pass_override = nullptr;
		heightmap = nullptr;
		normal = nullptr;
	}

	Shader* forward_pass_override;
	Shader* shadow_pass_override;

	
	// Store's the specular component of the material. 
	// Unless defined otherwise, this is a 3 component 8bit color with black as it's default.
	// If std::holds_alternative<Texture2D*>(specular) holds true, it is assumed the pointer is valid.
	std::variant<glm::vec3, Texture2D*> specular = glm::vec3(1);

	Texture2D* diffuse;
	Texture2D* heightmap;
	Texture2D* normal;

	float shininess;
};