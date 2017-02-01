#pragma once
#include <string>

class Mesh;

/*
	Contains implementations of loaders for various model formats
	TODO: Implement automatic recognition of file extensions
*/
namespace MeshLoaders {
	
	/*
		Loads a file and set's up a mesh using the provided mesh loading function.
	*/
	void loadFromFile(Mesh& mesh, const std::string& file, void(*loader)(Mesh&, const std::string&));


	/*
		Loads a mesh in .ply format
	*/
	void loadPly(Mesh& mesh, const std::string& ply);

}