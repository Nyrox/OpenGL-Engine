#include "Texture3D.h"
#include <stb_image.h>
#include <iostream>
#include <algorithm>


Texture3D::Texture3D(TextureSettings settings) : TextureBase(gl::TEXTURE_3D, settings) {
	updateParameters();
}

Texture3D::Texture3D(std::array<std::string, 6> files, GLenum internalFormat, TextureSettings settings) : Texture3D(settings) {

}


void Texture3D::allocate(GLenum format, glm::vec2 size) {

}


void Texture3D::updateParameters() {

}


void Texture3D::loadFromFile(std::array<std::string, 6> files, GLenum pixelFormat) {

}
