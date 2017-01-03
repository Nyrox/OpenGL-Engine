#include "Sprite.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Sprite::Sprite(uint32_t _width, uint32_t _height, Texture* texture) : width(_width), height(_height), texture(texture), yaw(0) {

	initRenderData();
}

void Sprite::draw(Shader& shader) {
	shader.bind();
	
	shader.setUniform("model", glm::rotate(glm::translate(glm::mat4(), position), glm::radians(yaw), { 0, 1, 0 }));
	shader.setUniform("diffuse", 0);

	
	texture->bind(0);

	gl::BindVertexArray(this->vao);
	gl::DrawArrays(gl::TRIANGLE_STRIP, 0, 4);
	gl::BindVertexArray(0);
}

void Sprite::initRenderData() {
	GLuint vbo;

	GLfloat vertices[] = {
		0, 0, 0,			0, 0, 1,	0, 0,
		width, 0, 0,		0, 0, 1,	1, 0,
		0, height, 0,		0, 0, 1,	0, 1,
		width, height, 0,	0, 0, 1,	1, 1
	};

	gl::GenVertexArrays(1, &this->vao);
	gl::GenBuffers(1, &vbo);
	
	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(vertices), vertices, gl::STATIC_DRAW);

	gl::BindVertexArray(this->vao);

	// Positions
	gl::VertexAttribPointer(0, 3, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)0);
	gl::EnableVertexAttribArray(0);

	// Normals
	gl::VertexAttribPointer(1, 3, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(1);

	// UVS
	gl::VertexAttribPointer(2, 2, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(2);

	gl::BindVertexArray(0);
}