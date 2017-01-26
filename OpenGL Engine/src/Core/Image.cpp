#include "Image.h"
#include <stb_image.h>

void Image::loadFromFile(const std::string& filename) {

	data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
}

uint8_t Image::getPixel(uint32_t x, uint32_t y, uint8_t channel) const {
	assert(channel < channels);


	return data[x + channel + y * width * channels];
}