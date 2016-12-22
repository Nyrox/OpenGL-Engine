#include "Sprite.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Sprite::Sprite(uint32_t _width, uint32_t _height, Texture* texture) : width(_width), height(_height), texture(texture), yaw(0) {

	initRenderData();
}

void Sprite::draw(Shader& shader) {
	shader.bind();
	
	shader.setUniform("model", glm::translate(glm::mat4(), position));
	shader.setUniform("diffuse", 0);

	
	texture->bind(0);

	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Sprite::initRenderData() {
	GLuint vbo;

	GLfloat vertices[] = {
		0, 0, 0,			0, 0, 1,	0, 0,
		width, 0, 0,		0, 0, 1,	1, 0,
		0, height, 0,		0, 0, 1,	0, 1,
		width, height, 0,	0, 0, 1,	1, 1
	};

	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->vao);

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// UVS
	glVertexAttribPointer(2, 2, GL_FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}