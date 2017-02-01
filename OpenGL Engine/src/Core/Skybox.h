#pragma once
#include <Core/Shader.h>
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


	void render(glm::mat4, glm::mat4);
private:
	Shader shader;

	GLuint id;
};