#include "Terrain.h"
#include <vector>

void Terrain::generateMeshFromFunction(std::function<float()> generator) {

	constexpr float step_size = 10;
	constexpr int width = 10;
	constexpr int height = 10;


	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for (int i = 0; i < width; i++) {
		for (int b = 0; b < height; b++) {
			// Generate vertex
			Vertex vertex;
			vertex.position = glm::vec3(i * step_size, generator(), b * step_size);
			vertex.normal = glm::vec3(0, 1, 0);
			vertex.uv = glm::vec2((1.f / width) * i, (1.f / height) * b);

			vertices.push_back(vertex);
		
			// While we are at it generate indices
			if (i >= width - 1 || b >= height - 1) {
				continue;
			}
			
			indices.push_back(b * height + i);
			indices.push_back(b * height + i + 1);
			indices.push_back((b + 1) * height + i);
			indices.push_back((b + 1) * height + i);
			indices.push_back(b * height + i + 1);
			indices.push_back((b + 1) * height + i + 1);
		}
	}

	mesh.loadFromMemory(vertices, indices);
}