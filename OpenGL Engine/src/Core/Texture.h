#pragma once
#include <gl_core_4_3.hpp>
#include <string>


/*
	Responsible for loading 2D textures.
	TODO: Rethink the purpose and implementation of this class.
*/
class Texture {
public:
	Texture();

	GLuint id;
	int32_t width, height;

	void loadFromFile(std::string filename, GLenum format);
	void bind(int tu);
};