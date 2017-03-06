#pragma once
#include <Core/Shader.h>
#include <Core/Mesh.h>
#include <array>

#include <Core/Texture3D.h>
#include <optional>

/*
	A skybox which can be used both for IBL and for skybox rendering
*/
class Skybox {
public:
	Skybox();

	/*
		Loads the 6 faces of a skybox and generates vertex buffers
		@[params]
		@paths: A fixed size array to the file locations. 
				Cubemap ordering follows OpenGL conventions.
	*/
	void loadFromFiles(const std::array<std::string, 6>& paths);
	void loadFromHDRFile(const std::string& file);


	void render(glm::mat4 view, glm::mat4 projection);
private:
	Mesh skyboxMesh;
	Shader shader;

	Texture3D cubemap;
	std::optional<Texture3D> convolutedCubemap;
};