#pragma once
#include <cassert>
#include <string>

class Texture;


class Image {
public:	
	void loadFromFile(const std::string& file);

	uint8_t getPixel(uint32_t x, uint32_t y, uint8_t channel) const;

	uint8_t* data;
	int32_t channels;
	int32_t width, height;
};