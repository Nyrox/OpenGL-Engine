#pragma once
#include <cassert>
#include <string>

class Texture;

/*
	Provides a way of storing, getting and manipulating image data cpu side.
	Only provides support for 8bit image data
*/
class Image {
public:	
	/*
		Loads an Image from file
	*/
	void loadFromFile(const std::string& file);

	/*
		Retrieves a given pixel from the Image
	*/
	uint8_t getPixel(uint32_t x, uint32_t y, uint8_t channel) const;

	uint8_t* data;
	int32_t channels;
	int32_t width, height;
};