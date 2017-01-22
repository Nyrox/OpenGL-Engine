#pragma once

#include <Shader.h>
#include <Texture.h>

/*
	Defines a sprite in § dimensional space
*/
class Sprite {
public:
	Sprite(uint32_t width, uint32_t height);

	void draw(Shader& shader);

	GLuint vao;

	glm::vec3 position;

	uint32_t width, height;
private:
	void initRenderData();

};
