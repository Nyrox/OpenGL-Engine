#pragma once
#include <gl_core_4_3.hpp>
#include <glm\glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <unordered_map>

/*
	POD Representation of a single glyph.	
*/
struct Glyph {
	GLuint textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};


class Font {
public:
	Font::Font();
	Font::Font(const std::string& file, uint8_t);
	~Font();

	void loadFromFile(const std::string& file, uint8_t fontSize);

	Glyph& getGlyph(uint8_t character);

private:
	FT_Library ft;
	FT_Face face;

	std::unordered_map<uint8_t, Glyph> characters;
};