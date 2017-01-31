#include "ImmediateDraw.h"
#include <iostream>

ImmediateDraw::ImmediateDraw() {

}

ImmediateDraw& ImmediateDraw::getInstance() {
	static ImmediateDraw _instance;
	return _instance;
}

GLuint ImmediateDraw::cachePlane(float width, float height) {
	GLuint vao, vbo;

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

	plane_cache[std::make_pair(width, height)] = vao;

	return vao;
}

GLuint ImmediateDraw::cacheCube(glm::vec3 size) {
	GLuint vao, vbo, ebo;

	GLfloat vertices[] = {
		// Front
		0.0, 0.0, 0.0,			0.0, 0.0, -1.0,		0.0, 0.0,
		size.x, 0.0, 0.0,		0.0, 0.0, -1.0,		1.0, 0.0,
		0.0, size.y, 0.0,		0.0, 0.0, -1.0,		0.0, 1.0,
		size.x, size.y, 0.0,	0.0, 0.0, -1.0,		1.0, 1.0,
		// Left
		0.0, 0.0, size.z,		-1.0, 0.0, 0.0,		0.0, 0.0,
		0.0, 0.0, 0.0,			-1.0, 0.0, 0.0,		1.0, 0.0,
		0.0, size.y, size.z,	-1.0, 0.0, 0.0,		0.0, 1.0,
		0.0, size.y, 0.0,		-1.0, 0.0, 0.0,		1.0, 1.0,
		// Top
		0.0, size.y, size.z,	0.0, 1.0, 0.0,		0.0, 0.0,
		size.x, size.y, size.z, 0.0, 1.0, 0.0,		1.0, 0.0,
		0.0, size.y, 0.0,		0.0, 1.0, 0.0,		0.0, 1.0,
		size.x, size.y, 0.0,	0.0, 1.0, 0.0,		1.0, 1.0,
		// Botom
		0.0, 0.0, size.z,		0.0, -1.0, 0.0,		0.0, 0.0,
		size.x, 0.0, size.z,	0.0, -1.0, 0.0,		1.0, 0.0,
		0.0, 0.0, 0.0,			0.0, -1.0, 0.0,		0.0, 1.0,
		size.x, 0.0, 0.0,		0.0, -1.0, 0.0,		1.0, 1.0,
		// Right
		size.x, 0.0, 0.0,		1.0, 0.0, 0.0,		0.0, 0.0,
		size.x, 0.0, size.z,	1.0, 0.0, 0.0,		1.0, 0.0,
		size.x, size.y, 0.0,	1.0, 0.0, 0.0,		0.0, 1.0,
		size.x, size.y, size.z, 1.0, 0.0, 0.0,		1.0, 1.0,
		// Back
		size.x, 0.0, size.z,	0.0, 0.0, 1.0,		0.0, 0.0,
		0.0, 0.0, size.z,		0.0, 0.0, 1.0,		1.0, 0.0,
		size.x, size.y, size.z, 0.0, 0.0, 1.0,		0.0, 1.0,
		0.0, size.y, size.z,	0.0, 0.0, 1.0,		1.0, 1.0
	};

	uint32_t indices[] = {
		// Front
		0, 1, 2, 2, 1, 3,
		// Left
		4, 5, 6, 6, 5, 7,
		// Top
		8, 9, 10, 10, 9, 11,
		// Bottom
		12, 13, 14, 14, 13, 15,
		// Right
		16, 17, 18, 18, 17, 19,
		// Back
		20, 21, 22, 22, 21, 23
	};

	gl::GenVertexArrays(1, &vao);
	gl::BindVertexArray(vao);

	gl::GenBuffers(1, &vbo);
	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(vertices), vertices, gl::STATIC_DRAW);

	gl::GenBuffers(1, &ebo);
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, ebo);
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, ARRAYSIZE(indices) * sizeof(uint32_t), indices, gl::STATIC_DRAW);


	
	// Positions
	gl::VertexAttribPointer(0, 3, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)0);
	gl::EnableVertexAttribArray(0);
	// Normals
	gl::VertexAttribPointer(1, 3, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(1);
	// Texture Coordsf
	gl::VertexAttribPointer(2, 2, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(2);

	cube_cache[std::make_tuple(size.x, size.y, size.z)] = vao;
	return vao;
}

void ImmediateDraw::drawPlane(float width, float height) {
	ImmediateDraw& _this = ImmediateDraw::getInstance();

	GLuint vao;
	auto cached = _this.plane_cache.find(std::make_pair(width, height));
	if (cached == _this.plane_cache.end()) {
		vao = _this.cachePlane(width, height);
	}
	else {
		vao = cached->second;
	}

	gl::BindVertexArray(vao);
	gl::DrawArrays(gl::TRIANGLE_STRIP, 0, 4);
	gl::BindVertexArray(0);
}

void ImmediateDraw::drawCube(glm::vec3 size) {
	ImmediateDraw& _this = ImmediateDraw::getInstance();

	GLuint vao;
	auto cached = _this.cube_cache.find(std::make_tuple(size.x, size.y, size.z));
	if (cached == _this.cube_cache.end()) {
		vao = _this.cacheCube(size);
	}
	else {
		vao = cached->second;
	}

	gl::BindVertexArray(vao);
	gl::DrawElements(gl::TRIANGLES, 36, gl::UNSIGNED_INT, 0);
	gl::BindVertexArray(0);
}