#include "Cube.h"
#include <Shader.h>

#include <glm/gtc/matrix_transform.hpp>

void Cube::draw(Shader& shader) {

	shader.setUniform("model", glm::rotate(glm::translate(glm::mat4(), position), 45.f, { 0.5, 0.5, 0 }));

	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Cube::initRenderData() {
	GLuint vbo, ebo;
	
	GLfloat vertices[] = {
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 1.0,
		1.0, 1.0, 1.0
	};

	GLuint indices[] = {
		0, 1, 2,
		1, 3, 2,
		1, 4, 2
	};

	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(GLfloat), (GLvoid*)0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
