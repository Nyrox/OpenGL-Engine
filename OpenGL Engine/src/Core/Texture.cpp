#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

namespace Refactor {

	Texture2D::Texture2D(bool t_mipmaps, GLenum t_texture_wrap, GLenum t_filtering) 
		: mipmaps(t_mipmaps), texture_wrap(t_texture_wrap), filtering(t_filtering) 
	{
		gl::CreateTextures(gl::TEXTURE_2D, 1, &handle);
	}

	Texture2D::~Texture2D() {
		gl::DeleteTextures(1, &handle);
	}

	void Texture2D::bind(uint32_t texture_unit) {
		gl::ActiveTexture(gl::TEXTURE0 + texture_unit);
		gl::BindTexture(gl::TEXTURE_2D, handle);
	}

	void Texture2D::allocate(GLenum format, glm::vec2 size) {
		gl::TextureStorage2D(handle, 1, format, size.x, size.y);
	}

}

Texture::Texture() {
	gl::GenTextures(1, &id);
}

void Texture::loadFromFile(std::string filename, GLenum internal_format) {
	bind(0);

	int32_t n;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &n, 0);
	std::cout << "Channel count: " << n << " - for: " + filename << std::endl;

	GLenum format;
	switch (n) {
	case 4:
		format = gl::RGBA;
		break;
	case 3:
		format = gl::RGB;
		break;
	case 1:
		format = gl::RED;
		break;
	}

	gl::TexImage2D(gl::TEXTURE_2D, 0, internal_format, width, height, 0, format, gl::UNSIGNED_BYTE, data);
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