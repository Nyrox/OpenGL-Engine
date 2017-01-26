#pragma once
#include <Mesh.h>
#include <functional>


class Image;

class Terrain {
public:
	Terrain(float width, float height);

	void generateMeshFromFunction(std::function<float(const Terrain&, uint32_t, uint32_t)> generator);
	void generateMeshFromHeightmap(const Image& heightmap, float heightmapIntensity = 0.25);

	Mesh mesh;

	float width, height;
	uint32_t segments_x, segments_y;
private:
	

};