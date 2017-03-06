#pragma once
#include <Core/TextureBase.h>
#include <glm/glm.hpp>
#include <string>
#include <array>
/*
3D Cubemaps
Currently has a soft requirement that all 6 sides must be the same size
*/
class Texture3D : public TextureBase {
public:
	// Default constructs a texture
	Texture3D(TextureSettings settings = TextureSettings());

	// Constructs a texture and loads a texture from file in one call
	Texture3D(std::array<std::string, 6> files, GLenum internalFormat, TextureSettings settings = TextureSettings());
	Texture3D(const std::string& file, GLenum internalFormat);

	// Allocates space for a texture, ready to be used with a framebuffer or to be loaded in to from memory
	// INFO: When loading from file, you should not call this method
	void allocate(GLenum internalFormat, glm::vec2 size);
	
	// Update texture parameters
	void updateParameters();

	// Load textures from file
	// If pixelFormat is not provided, it will attempt to guess it 
	void loadFromFiles(std::array<std::string, 6> files, GLenum internalFormat, GLenum pixelFormat = 0);

	// Loads an HDR Image from file and renders it to a cubemap
	void loadFromHDRFile(const std::string& file, GLenum internalFormat);

	int32_t width, height;
private:
	static uint32_t impl_getMipmapLevels(bool useMipmap, uint32_t width, uint32_t height) noexcept;
};