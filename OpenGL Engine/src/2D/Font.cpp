#include "Font.h"

#include <iostream>

Font::Font() {
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
}

Font::Font(const std::string& file, uint8_t fontSize) : Font() {
	loadFromFile(file, fontSize);
}

Font::~Font() {
	FT_Done_FreeType(ft);
}

void Font::loadFromFile(const std::string& file, uint8_t fontSize) {
	GLint old;
	gl::GetIntegerv(gl::UNPACK_ALIGNMENT, &old);
	gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);

	if (FT_New_Face(ft, file.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);
	
	for (uint8_t c = 0; c < 255; c++) {
		std::cout << "Loading glyph: " << std::to_string(c) << "\n";
		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load glyph: " << c << std::endl;
			continue;
		}

		GLuint texture;
		gl::CreateTextures(gl::TEXTURE_2D, 1, &texture);
		gl::TextureStorage2D(texture, 1, gl::R8, face->glyph->bitmap.width, face->glyph->bitmap.rows);

		gl::TextureParameteri(texture, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
		gl::TextureParameteri(texture, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
		gl::TextureParameteri(texture, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::TextureParameteri(texture, gl::TEXTURE_MAG_FILTER, gl::LINEAR);

		Glyph glyph = {
			texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x
		};
		characters.insert(std::make_pair(c, glyph));
	}

	FT_Done_Face(face);

	gl::PixelStorei(gl::UNPACK_ALIGNMENT, old);
}

Glyph& Font::getGlyph(uint8_t character) {
	return characters[character];
}