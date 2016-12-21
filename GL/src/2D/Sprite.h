#pragma once

#include <Shader.h>
#include <Texture.h>

/*
	Defines a sprite in § dimensional space
*/
class Sprite {
public:
	Sprite(uint32_t width, uint32_t height, Texture* texture);

	void draw(Shader& shader);


	GLuint vao;

	glm::vec3 position;
	float yaw;

	uint32_t width, height;
private:
	void initRenderData();

	Texture* texture;
};
