#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <algorithm>


TextureSettings::TextureSettings(MipmapModes t_mipmapMode, TextureWrapModes t_textureWrapMode, FilteringModes t_filteringMode, float t_anisotropicFilteringLevel) 
	: mipmapMode(t_mipmapMode), textureWrapMode(t_textureWrapMode), filteringMode(t_filteringMode), anisotropicFilteringLevel(t_anisotropicFilteringLevel) {
}




Texture2D::Texture2D(TextureSettings t_settings) : settings(t_settings) {
	gl::CreateTextures(gl::TEXTURE_2D, 1, &handle);
	updateParameters();
}

Texture2D::Texture2D(const std::string& textureFile, GLenum internalFormat, TextureSettings settings) : Texture2D(settings) {
	int32_t n;
	unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &n, 0);

	GLenum pixelFormats[] = { 0, gl::RED, gl::RG, gl::RGB, gl::RGBA };
	GLenum pixelFormat = pixelFormats[n];

	allocate(internalFormat, { width, height });
	gl::TextureSubImage2D(handle, 0, 0, 0, width, height, pixelFormat, gl::UNSIGNED_BYTE, data);
	gl::GenerateTextureMipmap(handle);
}

Texture2D::Texture2D(Texture2D&& other) {

}

Texture2D::~Texture2D() {
	gl::DeleteTextures(1, &handle);
}

void Texture2D::bind(uint32_t texture_unit) {
	gl::ActiveTexture(gl::TEXTURE0 + texture_unit);
	gl::BindTexture(gl::TEXTURE_2D, handle);
}

void Texture2D::updateParameters() {
	gl::TextureParameteri(handle, gl::TEXTURE_WRAP_S, settings.textureWrapMode);
	gl::TextureParameteri(handle, gl::TEXTURE_WRAP_T, settings.textureWrapMode);
	gl::TextureParameteri(handle, gl::TEXTURE_MIN_FILTER, settings.filteringMode);
	gl::TextureParameteri(handle, gl::TEXTURE_MAG_FILTER, gl::LINEAR);

	if (gl::exts::var_EXT_texture_filter_anisotropic && std::floor(settings.anisotropicFilteringLevel) != 0) {
		gl::TextureParameterf(handle, gl::TEXTURE_MAX_ANISOTROPY_EXT, settings.anisotropicFilteringLevel);
	}
}

void Texture2D::allocate(GLenum format, glm::vec2 size) {

	uint8_t levels = settings.mipmapMode ? 1 + (uint8_t)std::floor(std::log2(std::max(size.x, size.y))) : 1;

	gl::TextureStorage2D(handle, levels, format, size.x, size.y);
	width = size.x;
	height = size.y;
	internalFormat = format;
}

void Texture2D::loadFromFile(const std::string& file, GLenum pixelFormat) {
	int32_t n, t_width, t_height;
	unsigned char* data = stbi_load(file.c_str(), &t_width, &t_height, &n, 0);
	std::cout << "Channel count: " << n << " - for: " << file << std::endl;

	if (t_width != width || t_height != height) {
		std::cout << "Error while loading texture: Image dimensions do not match size provided with allocation." << std::endl;
		throw std::exception();
	}

	gl::TextureSubImage2D(handle, 0, 0, 0, width, height, pixelFormat, gl::UNSIGNED_BYTE, data);
	if (settings.mipmapMode) gl::GenerateTextureMipmap(handle);
}
