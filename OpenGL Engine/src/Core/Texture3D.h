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
	// Essentially equal to creating a texture and calling 'allocate' and 'loadFromFiles'.
	Texture3D(std::array<std::string, 6> files, GLenum internalFormat, TextureSettings settings = TextureSettings());

	// Allocates space for a texture, ready to be used with a framebuffer or to be loaded in to from memory
	void allocate(GLenum format, glm::vec2 size);

	// Update texture parameters
	void updateParameters();

	// Loads a texture from file in to space allocated with 'allocate'
	void loadFromFile(std::array<std::string, 6> files, GLenum pixelFormat = 0);


	int32_t width, height;
private:

};