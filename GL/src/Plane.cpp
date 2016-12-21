#include "Plane.h"

#include <Shader.h>
#include <glm/gtc/matrix_transform.hpp>

Plane::Plane() {
	scale = 1;
}

void Plane::draw(Shader& shader) {
	shader.bind();
	shader.setUniform("model", glm::translate(glm::scale(glm::mat4(), { scale, scale, scale }), position));
	shader.setUniform("material", material);

	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Plane::initRenderData() {
	GLuint vbo;

	GLfloat vertices[] = {
		-5, 0, -5, 0, 1, 0, 0, 0,
		5,  0, -5, 0, 1, 0, 1, 0,
		-5, 0, 5, 0, 1, 0, 0, 1,
		-5, 0, 5, 0, 1, 0, 0, 1,
		5,  0, -5, 0, 1, 0, 1, 0,
		5,  0, 5, 0, 1, 0, 1, 1
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

	// Texture UV's
	glVertexAttribPointer(2, 2, GL_FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

