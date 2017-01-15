#pragma once
#include <Mesh.h>
#include <functional>


class Terrain {
public:

	void generateMeshFromFunction(std::function<float()> generator);


	Mesh mesh;
private:
	

};