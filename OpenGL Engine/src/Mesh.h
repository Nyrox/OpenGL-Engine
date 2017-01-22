#pragma once
#include <string>
#include <glm/glm.hpp>

#include <Material.h>
#include <vector>

#include <Shader.h>

struct Vertex {
	Vertex() {}
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv) : position(position), normal(normal), uv(uv) { }
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Mesh {
public:
	Mesh();

	GLuint vao;

	void draw(Shader& shader);
	void loadFromFile(std::string path);
	void loadFromMemory(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

	glm::vec3 position;
	Material material;


	glm::vec3 scale;

	// Trivia -- 
	uint32_t vertexCount, faceCount;

private:
	
	void initRenderData();

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

};

