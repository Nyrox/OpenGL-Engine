#pragma once
#include <gl_core_4_3.hpp>
#include <glm/glm.hpp>

#include <Material.h>

class Shader;

class Cube {
public:
	Cube(); 

	void draw(Shader& shader);
	void initRenderData();

	glm::vec3 position;
	float scale;
	
	Material material;
private:
	GLuint vao;

	
};