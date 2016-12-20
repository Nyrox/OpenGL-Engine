#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() {
	glGenTextures(1, &id);
}

int to = 0;

void Texture::loadFromFile(std::string filename, GLenum format) {
	bind(to);
	to += 1;

	int32_t n;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &n, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(int tu) {
	glActiveTexture(GL_TEXTURE0 + tu);
	
	glBindTexture(GL_TEXTURE_2D, this->id);
}