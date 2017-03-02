#pragma once
#include <Core/Shader.h>
#include <Core/Mesh.h>

#include <array>


/*
	Renders a skybox from a cubemap
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


	void render(glm::mat4 view, glm::mat4 projection);
private:
	Mesh skyboxMesh;
	Shader shader;

	GLuint id;
};