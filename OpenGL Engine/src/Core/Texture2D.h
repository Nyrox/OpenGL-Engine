#pragma once
#include <Core/TextureBase.h>
#include <string>

/*
2D Textures
*/
class Texture2D : public TextureBase {
public:
	// Default constructs a texture
	Texture2D(TextureSettings settings = TextureSettings());

	// Constructs a texture and loads a texture from file in one call
	// Essentially equal to creating a texture and calling 'allocate' and 'loadFromFile'.
	Texture2D(const std::string& textureFile, GLenum internalFormat, TextureSettings settings = TextureSettings());
	
	// Allocates space for a texture, ready to be used with a framebuffer or to be loaded in to from memory
	void allocate(GLenum format, int32_t width, int32_t height);

	// Update texture parameters
	void updateParameters();
	
	// Loads a texture from file in to space allocated with 'allocate'
	void loadFromFile(const std::string& file, GLenum pixelFormat = 0);

	int32_t width, height;
private:
	uint8_t* impl_loadFile(const std::string& file, int32_t& width, int32_t& height, int32_t& n) const;
	static uint32_t impl_getMipmapLevels(bool useMipmap, uint32_t width, uint32_t height) noexcept;

	
};


