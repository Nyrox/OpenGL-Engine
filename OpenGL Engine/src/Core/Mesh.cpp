#include "Mesh.h"
#include <Core\MeshLoaders.h>

#include <iostream>

void Mesh::draw() {
	if (vao == 0) {
		std::cout << "Error::Mesh::draw: Tried to draw mesh without a valid VAO" << std::endl;
		return;
	}

	gl::BindVertexArray(vao);
	gl::DrawElements(gl::TRIANGLES, index_count, gl::UNSIGNED_INT, 0);
	gl::BindVertexArray(0);
}

void Mesh::loadFromFile(const std::string& path) {
	MeshLoaders::loadFromFile(*this, path, MeshLoaders::loadPly);
}

void Mesh::initRenderData(std::vector<Vertex> vertices, std::vector<uint32_t> indices) {

	index_count = indices.size();

	gl::GenVertexArrays(1, &this->vao);
	gl::GenBuffers(1, &vbo);
	gl::GenBuffers(1, &ebo);

	gl::BindVertexArray(this->vao);

	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, ebo);
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), gl::STATIC_DRAW);

	
	// Positions
	gl::VertexAttribPointer(0, 3, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)0);
	gl::EnableVertexAttribArray(0);
	// Normals
	gl::VertexAttribPointer(1, 3, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(1);
	// Texture Coords
	gl::VertexAttribPointer(2, 2, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(2);

	gl::BindVertexArray(0);
}