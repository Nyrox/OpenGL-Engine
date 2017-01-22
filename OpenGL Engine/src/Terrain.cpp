#include "Terrain.h"
#include <vector>

Terrain::Terrain(float t_width, float t_height) : width(t_width), height(t_height) { 

}

void Terrain::generateMeshFromFunction(std::function<float()> generator) {


	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	uint32_t segments_x = 32;
	uint32_t segments_y = 32;

	for (int i = 0; i < segments_x; i++) {
		for (int b = 0; b < segments_y; b++) {
			// Generate vertex
			Vertex vertex;
			vertex.position = glm::vec3(i * (width / segments_x), generator(), b * (height / segments_y));
			vertex.normal = glm::vec3(0, 1, 0);
			vertex.uv = glm::vec2((1.f / segments_x) * i, (1.f / segments_y) * b);

			vertices.push_back(vertex);
		
			// While we are at it generate indices
			if (i >= segments_x - 1 || b >= segments_y - 1) {
				continue;
			}
			
			indices.push_back(b * segments_y + i);
			indices.push_back(b * segments_y + i + 1);
			indices.push_back((b + 1) * segments_y + i);
			indices.push_back((b + 1) * segments_y + i);
			indices.push_back(b * segments_y + i + 1);
			indices.push_back((b + 1) * segments_y + i + 1);
		}
	}

	mesh.loadFromMemory(vertices, indices);
}