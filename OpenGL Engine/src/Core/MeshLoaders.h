#pragma once

#include <string>

/*
	Contains implementations of loaders for various model formats
	TODO: Implement automatic recognition of file extensions
*/

class Mesh;

namespace MeshLoaders {
	
	void loadFromFile(Mesh& mesh, const std::string& file, void(*loader)(Mesh&, const std::string&));

	// Loaders

	void loadPly(Mesh& mesh, const std::string& ply);

}