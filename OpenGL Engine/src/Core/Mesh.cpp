#include "Mesh.h"
#include <Core\MeshLoaders.h>
#include <glm\gtx\transform.hpp>
#include <iostream>

Mesh::Mesh(const std::string& file) {
	loadFromFile(file);
}

void Mesh::draw() const {
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
	vertex_count = vertices.size();
	index_count = indices.size();

	gl::CreateVertexArrays(1, &this->vao);
	gl::CreateBuffers(1, &vbo);
	gl::CreateBuffers(1, &ebo);

	gl::BindVertexArray(this->vao);

	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::NamedBufferData(vbo, sizeof(Vertex) * vertices.size(), vertices.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, ebo);
	gl::NamedBufferData(ebo, sizeof(uint32_t) * indices.size(), indices.data(), gl::STATIC_DRAW);


	// Positions
	gl::VertexAttribPointer(0, 3, gl::FLOAT, 0, sizeof(Vertex), (GLvoid*)(0));
	gl::EnableVertexAttribArray(0);
	// Normals
	gl::VertexAttribPointer(1, 3, gl::FLOAT, 0, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, normal)));
	gl::EnableVertexAttribArray(1);
	// Texture Coords
	gl::VertexAttribPointer(2, 2, gl::FLOAT, 0, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, uv)));
	gl::EnableVertexAttribArray(2);
	// Tangents
	gl::VertexAttribPointer(3, 3, gl::FLOAT, 0, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, tangent)));
	gl::EnableVertexAttribArray(3);

	gl::BindVertexArray(0);
}

// STATIC
Mesh Mesh::generatePlane(glm::vec2 size) {
	Mesh mesh;

	std::vector<Vertex> vertices = {
		Vertex { glm::vec3(0, 0, 0),			glm::vec3(0, 0, -1), glm::vec2(0, 0) },
		Vertex { glm::vec3(size.x, 0, 0),		glm::vec3(0, 0, -1), glm::vec2(1, 0) },
		Vertex { glm::vec3(0, size.y, 0),		glm::vec3(0, 0, -1), glm::vec2(0, 1) },
		Vertex { glm::vec3(size.x, size.y, 0),	glm::vec3(0, 0, -1), glm::vec2(1, 1) }
	};

	std::vector<uint32_t> indices = {
		0, 1, 2,
		2, 1, 3
	};

	mesh.initRenderData(vertices, indices);
	return mesh;
};

// STATIC
Mesh Mesh::generateCube(glm::vec3 size) {
	Mesh mesh;

	std::vector<Vertex> vertices = {
		Vertex { glm::vec3(0, 0, 0),				glm::vec3(0, 0, -1),	glm::vec2(0, 0) },
		Vertex { glm::vec3(size.x, 0, 0),			glm::vec3(0, 0, -1),	glm::vec2(1, 0) },
		Vertex { glm::vec3(0, size.y, 0),			glm::vec3(0, 0, -1),	glm::vec2(0, 1) },
		Vertex { glm::vec3(size.x, size.y, 0.0),	glm::vec3(0, 0, -1),	glm::vec2(1, 1) },
		Vertex { glm::vec3(0, 0, size.z),			glm::vec3(-1, 0, 0), 	glm::vec2(0, 0) },
		Vertex { glm::vec3(0, 0, 0),				glm::vec3(-1, 0, 0), 	glm::vec2(1, 0) },
		Vertex { glm::vec3(0, size.y, size.z),		glm::vec3(-1, 0, 0), 	glm::vec2(0, 1) },
		Vertex { glm::vec3(0, size.y, 0),			glm::vec3(-1, 0, 0), 	glm::vec2(1, 1) },
		Vertex { glm::vec3(0, size.y, size.z),		glm::vec3(0, 1, 0), 	glm::vec2(0, 0) },
		Vertex { glm::vec3(size.x, size.y, size.z),	glm::vec3(0, 1, 0), 	glm::vec2(1, 0) },
		Vertex { glm::vec3(0, size.y, 0.0),			glm::vec3(0, 1, 0), 	glm::vec2(0, 1) },
		Vertex { glm::vec3(size.x, size.y, 0.0),	glm::vec3(0, 1, 0), 	glm::vec2(1, 1) },
		Vertex { glm::vec3(0, 0, size.z),			glm::vec3(0, -1, 0), 	glm::vec2(0, 0) },
		Vertex { glm::vec3(size.x, 0.0, size.z),	glm::vec3(0, -1, 0), 	glm::vec2(1, 0) },
		Vertex { glm::vec3(0, 0, 0),				glm::vec3(0, -1, 0), 	glm::vec2(0, 1) },
		Vertex { glm::vec3(size.x, 0, 0),			glm::vec3(0, -1, 0), 	glm::vec2(1, 1) },
		Vertex { glm::vec3(size.x, 0, 0),			glm::vec3(1, 0, 0), 	glm::vec2(0, 0) },
		Vertex { glm::vec3(size.x, 0, size.z),		glm::vec3(1, 0, 0), 	glm::vec2(1, 0) },
		Vertex { glm::vec3(size.x, size.y, 0),		glm::vec3(1, 0, 0), 	glm::vec2(0, 1) },
		Vertex { glm::vec3(size.x, size.y, size.z),	glm::vec3(1, 0, 0), 	glm::vec2(1, 1) },
		Vertex { glm::vec3(size.x, 0, size.z),		glm::vec3(0, 0, 1), 	glm::vec2(0, 0) },
		Vertex { glm::vec3(0, 0, size.z),			glm::vec3(0, 0, 1), 	glm::vec2(1, 0) },
		Vertex { glm::vec3(size.x, size.y, size.z),	glm::vec3(0, 0, 1), 	glm::vec2(0, 1) },
		Vertex { glm::vec3(0, size.y, size.z),		glm::vec3(0, 0, 1), 	glm::vec2(1, 1) }
	};

	std::vector<uint32_t> indices = {
		0, 1, 2, 2, 1, 3,
		4, 5, 6, 6, 5, 7,
		8, 9, 10, 10, 9, 11,
		12, 13, 14, 14, 13, 15,
		16, 17, 18, 18, 17, 19,
		20, 21, 22, 22, 21, 23
	};

	mesh.initRenderData(vertices, indices);
	return mesh;
}