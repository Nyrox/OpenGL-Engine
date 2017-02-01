#pragma once
#include <Core/Model.h>
#include <functional>


class Image;

/*
	Handles loading and rendering of grid based terrain.
	Terrain can be generated from a procedual function or from loading a heightmap.
*/
class Terrain {
public:
	Terrain(float width, float height);

	/*
		Generates the mesh from a generator function by calling the function once for each grid node with the x and y indices of that node provided.
	*/
	void generateMeshFromFunction(std::function<float(const Terrain&, uint32_t, uint32_t)> generator);

	/*
		Generates the mesh from a heightmap.
		The heightmap is mapped onto the terrain 1 to 1, meaning that the texture spans exactly once over the terrain.
	*/
	void generateMeshFromHeightmap(const Image& heightmap, float heightmapIntensity = 0.25);

	Model model;
	float width, height;
	uint32_t segments_x, segments_y;
private:
	

};