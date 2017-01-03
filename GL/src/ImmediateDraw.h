#pragma once
#include <gl_core_4_3.hpp>

void drawPlane(float width, float height) {
	GLuint vbo, vao;

	GLfloat vertices[] = {
		0, 0, 0,			0, 0, -1,	0, 0, 
		width, 0, 0,		0, 0, -1,	1, 0,
		0, height, 0,		0, 0, -1,	0, 1,
		width, height, 0,	0, 0, 1,	1, 1
	};

	gl::GenVertexArrays(1, &vao);
	gl::GenBuffers(1, &vbo);

	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(vertices), vertices, gl::STATIC_DRAW);

	gl::BindVertexArray(vao);

	// Positions
	gl::VertexAttribPointer(0, 3, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)0);
	gl::EnableVertexAttribArray(0);

	// Normals
	gl::VertexAttribPointer(1, 3, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(1);

	// UVS
	gl::VertexAttribPointer(2, 2, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(2);

	gl::DrawArrays(gl::TRIANGLE_STRIP, 0, 4);
	gl::BindVertexArray(0);
}