#pragma once
#include <Mesh.h>
#include <functional>


class Terrain {
public:
	Terrain(float width, float height);

	void generateMeshFromFunction(std::function<float()> generator);


	Mesh mesh;

	float width;
	float height;
private:
	

};