#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <Material.h>

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

