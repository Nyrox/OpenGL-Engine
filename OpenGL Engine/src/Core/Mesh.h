#pragma once
#include <Core\Vertex.h>

#include <gl_core_4_3.hpp>
#include <vector>

// Generates and holds vertex and index buffers
// TODO: Maybe implement an std::optional to keep a cpu copy of the vertices and indices
class Mesh {
public:
	// Emit an OpenGL draw call
	// Does not have any side effects
	void draw();

	// Load a mesh from file
	// @[Params]
	// @file: The path to the file to load
	void loadFromFile(const std::string& file);

	// Load a mesh from arrays of mesh data and generate vbo's and vao's
	// @[Params]
	// @vertices: Array of vertices
	// @indices: Array of indices
	void initRenderData(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

	static Mesh generateCube(glm::vec3 size);
	static Mesh generatePlane(glm::vec2 size);

	uint32_t vertex_count, index_count;
	GLuint vao, vbo, ebo;
};