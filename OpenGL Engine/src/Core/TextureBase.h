#pragma once
#include <gl_core_4_3.hpp>
#include <cinttypes>

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


class TextureBase {
public:
	TextureBase(GLenum target, TextureSettings t_settings = TextureSettings());
	TextureBase(TextureBase&& other);
	virtual ~TextureBase();

	void bind(uint32_t textureUnit) const noexcept;

	TextureSettings settings;
	GLuint handle;
protected:
	TextureBase(const TextureBase& other) = delete;
	TextureBase& operator=(const TextureBase& other) = delete;

	GLenum target;
	GLenum internalFormat;

	static constexpr GLenum pixelFormats[] = { 0, gl::RED, gl::RG, gl::RGB, gl::RGBA };
};