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
	GLuint vao;

	void draw(Shader& shader);
	void loadFromFile(std::string path);

	glm::vec3 position;
	Material material;




	// Trivia -- 
	uint32_t vertexCount, faceCount;

private:
	void initRenderData();

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

};

