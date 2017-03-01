#pragma once
#include <gl_core_4_3.hpp>
#include <glm\glm.hpp>
#include <string>

enum MipmapModes { NoMipmaps = 0, GenerateMipmaps };
enum TextureWrapModes { Repeat = gl::REPEAT, RepeatMirrored = gl::MIRRORED_REPEAT, ClampToEdge = gl::CLAMP_TO_EDGE, ClampToBorder = gl::CLAMP_TO_BORDER };
enum FilteringModes { Nearest = gl::NEAREST, Linear = gl::LINEAR, Bilinear = gl::LINEAR_MIPMAP_LINEAR };

struct TextureSettings {
	
	TextureSettings(MipmapModes mipmapMode = NoMipmaps, TextureWrapModes textureWrapMode = Repeat, FilteringModes filteringMode = Nearest, float anisotropicFilteringLevel = 0.f);
	
	MipmapModes mipmapMode = NoMipmaps;
	TextureWrapModes textureWrapMode = Repeat;
	FilteringModes filteringMode = Nearest;
	float anisotropicFilteringLevel = 0.f;
};


class Texture2D {
public:
	Texture2D(TextureSettings settings = TextureSettings());
	Texture2D(const std::string& textureFile, GLenum internalFormat, TextureSettings settings = TextureSettings());

	Texture2D(Texture2D&& other);

	~Texture2D();

	void bind(uint32_t texture_unit);
	void allocate(GLenum format, glm::vec2 size);
	void updateParameters();

	/*
		Loads a texture from file
		Calling this without calling allocate first will result in a crash
	*/
	void loadFromFile(const std::string& file, GLenum pixelFormat = 0);

	TextureSettings settings;

	GLenum internalFormat;
	GLuint handle;
	int32_t width, height;

private:
	Texture2D(const Texture2D& other) = delete;

};


