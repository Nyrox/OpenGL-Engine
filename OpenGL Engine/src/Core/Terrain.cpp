#include "Terrain.h"
#include <Core\Image.h>

#include <vector>

Terrain::Terrain(Material& material, float t_width, float t_height) : model(material, std::make_shared<Mesh>()), width(t_width), height(t_height) { 

}

void Terrain::generateMeshFromHeightmap(const Image& heightmap, float heightmapIntensity) {
	generateMeshFromFunction([&](const Terrain& terrain, uint32_t x, uint32_t y) {
		return (heightmap.getPixel(heightmap.width / segments_x * x, heightmap.height / segments_y * y, 0) - 128) * heightmapIntensity;
	});
}

void Terrain::generateMeshFromFunction(std::function<float(const Terrain&, uint32_t, uint32_t)> generator) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	segments_x = 32;
	segments_y = 32;

	for (int i = 0; i < segments_x; i++) {
		for (int b = 0; b < segments_y; b++) {
			// Generate vertex
			Vertex vertex;
			vertex.position = glm::vec3(i * (width / segments_x), generator(*this, i, b), b * (height / segments_y));
			vertex.normal = glm::vec3(0, 1, 0);
			vertex.uv = glm::vec2((1.f / segments_x) * i, (1.f / segments_y) * b);
			vertex.tangent = glm::vec3(1, 0, 0);

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

	model.mesh->initRenderData(vertices, indices);
}