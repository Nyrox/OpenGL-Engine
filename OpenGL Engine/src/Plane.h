#pragma once
#include <gl_core_4_3.hpp>
#include <glm/glm.hpp>

#include <Core/Material.h>

class Shader;


/*
	Defines a plane with a default size of 100x100	
*/

class Plane {
public:
	Plane();

	void draw(Shader& shader);
	void initRenderData();

	glm::vec3 position;
	float scale;

	Material material;

private:
	GLuint vao;
};

