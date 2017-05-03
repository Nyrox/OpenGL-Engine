#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <algorithm>



Texture2D::Texture2D(TextureSettings settings) : TextureBase(gl::TEXTURE_2D, settings) {
	updateParameters();
}

Texture2D::Texture2D(const std::string& textureFile, GLenum internalFormat, TextureSettings settings) : Texture2D(settings) {
	int32_t n;
	
	if (internalFormat == gl::RGB16F || internalFormat == gl::RGB32F) {
		float* data = stbi_loadf(textureFile.c_str(), &width, &height, &n, 0);
		allocate(internalFormat, width, height);
		gl::TextureSubImage2D(handle, 0, 0, 0, width, height, TextureBase::pixelFormats[n], gl::FLOAT, data);
		gl::GenerateTextureMipmap(handle);
		stbi_image_free(data);
		return;
	}

	uint8_t* data = impl_loadFile(textureFile, width, height, n);
	
	allocate(internalFormat, width, height);	
	gl::TextureSubImage2D(handle, 0, 0, 0, width, height, TextureBase::pixelFormats[n], gl::UNSIGNED_BYTE, data);
	gl::GenerateTextureMipmap(handle);
	stbi_image_free(data);
	return;
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

void Texture2D::allocate(GLenum format, int32_t t_width, int32_t t_height) {	
	gl::TextureStorage2D(handle, impl_getMipmapLevels(settings.mipmapMode, t_width, t_height), format, t_width, t_height);
}

void Texture2D::loadFromFile(const std::string& file, GLenum pixelFormat) {
	int32_t width, height, n;
	uint8_t* data = impl_loadFile(file, width, height, n);
	
	if (pixelFormat == 0) pixelFormat = pixelFormats[n];

	gl::TextureSubImage2D(handle, 0, 0, 0, width, height, pixelFormat, gl::UNSIGNED_BYTE, data);
	if (settings.mipmapMode) gl::GenerateTextureMipmap(handle);
}


/*
	Texture 2D Implementation
*/
#include <filesystem>
namespace std { namespace filesystem = experimental::filesystem; }

uint8_t* Texture2D::impl_loadFile(const std::string& file, int32_t& width, int32_t& height, int32_t& n) const {
	if (!std::filesystem::exists(std::filesystem::path(file))) { throw std::exception(); }
	return stbi_load(file.c_str(), &width, &height, &n, 0);
}

uint32_t Texture2D::impl_getMipmapLevels(bool useMipmaps, uint32_t width, uint32_t height) noexcept {
	return useMipmaps ? 1 + std::floor(std::log2(std::max(width, height))) : 1;
}