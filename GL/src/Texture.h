#pragma once

#include <GL/glew.h>
#include <string>


class Texture {
public:
	Texture();

	GLuint id;
	int32_t width, height;

	void loadFromFile(std::string filename, GLenum format);
	void bind(int tu);
};