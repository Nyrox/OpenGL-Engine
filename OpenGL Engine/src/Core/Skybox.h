#pragma once
#include <Core/Shader.h>

#include <array>
#include <string>


class Skybox {
public:
	Skybox();

	void loadFromFiles(const std::array<std::string, 6>& paths);

	void render(glm::mat4, glm::mat4);
private:
	Shader shader;


	GLuint id;

};