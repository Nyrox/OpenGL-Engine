#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() {
	gl::GenTextures(1, &id);
}

void Texture::loadFromFile(std::string filename, GLenum format) {
	bind(0);

	int32_t n;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &n, 0);
	gl::TexImage2D(gl::TEXTURE_2D, 0, format, width, height, 0, format, gl::UNSIGNED_BYTE, data);
	gl::GenerateMipmap(gl::TEXTURE_2D);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::REPEAT);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::REPEAT);

	gl::BindTexture(gl::TEXTURE_2D, 0);
}

void Texture::bind(int tu) {
	gl::ActiveTexture(gl::TEXTURE0 + tu);
	
	gl::BindTexture(gl::TEXTURE_2D, this->id);
}