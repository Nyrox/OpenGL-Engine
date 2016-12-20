#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader;

class Cube {
public:
	
	void draw(Shader& shader);
	void initRenderData();

	glm::vec3 position;
	glm::vec3 color;
private:
	GLuint vao;

	
};