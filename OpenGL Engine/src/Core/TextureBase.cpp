#include "TextureBase.h"


/*
	Texture Settings
*/


TextureSettings::TextureSettings(MipmapModes t_mipmapMode, TextureWrapModes t_textureWrapMode, FilteringModes t_filteringMode, float t_anisotropicFilteringLevel)
	: mipmapMode(t_mipmapMode), textureWrapMode(t_textureWrapMode), filteringMode(t_filteringMode), anisotropicFilteringLevel(t_anisotropicFilteringLevel) {
}


/*
	Texture Base
*/


TextureBase::TextureBase(GLenum t_target, TextureSettings t_settings) : target(t_target), settings(t_settings) {
	gl::CreateTextures(target, 1, &handle);
}

TextureBase::TextureBase(TextureBase&& other) {
	settings = other.settings;
	handle = other.handle;
	target = other.target;

	other.handle = 0;
}

TextureBase::~TextureBase() {
	gl::DeleteTextures(1, &handle);
}

void TextureBase::bind(uint32_t textureUnit) const noexcept {
	gl::ActiveTexture(gl::TEXTURE0 + textureUnit);
	gl::BindTexture(target, handle);
}