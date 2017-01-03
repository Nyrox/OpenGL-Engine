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

	gl::BindVertexArray(this->vao);
	gl::DrawArrays(gl::TRIANGLES, 0, 6);
	gl::BindVertexArray(0);
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

	// Texture UV's
	gl::VertexAttribPointer(2, 2, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(2);

	gl::BindVertexArray(0);
}

