#include "Texture3D.h"
#include <Core/Texture2D.h>
#include <Core/Framebuffer.h>
#include <Core/Shader.h>
#include <Core/Mesh.h>

#include <stb_image.h>
#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

Texture3D::Texture3D(TextureSettings settings) : TextureBase(gl::TEXTURE_CUBE_MAP, settings) {
	updateParameters();
}

Texture3D::Texture3D(std::array<std::string, 6> files, GLenum t_internalFormat, TextureSettings settings) : Texture3D(settings) {
	this->internalFormat = t_internalFormat;
	loadFromFiles(files, internalFormat);
}


void Texture3D::allocate(GLenum t_internalFormat, glm::vec2 size) {
	internalFormat = t_internalFormat;
	const int32_t levels = Texture3D::impl_getMipmapLevels(settings.mipmapMode, size.x, size.y);
	gl::TextureStorage2D(handle, levels, internalFormat, size.x, size.y);
}


void Texture3D::updateParameters() {

}


void Texture3D::loadFromFiles(std::array<std::string, 6> files, GLenum t_internalFormat, GLenum t_pixelFormat) {
	this->internalFormat = t_internalFormat;

	int32_t t_width, t_height, n;
	uint8_t* data;
	
	for (uint32_t i = 0; i < files.size(); i++) {
		data = stbi_load(files[i].c_str(), &t_width, &t_height, &n, 0);

		if (i == 0) {
			allocate(internalFormat, { t_width, t_height });
			this->width = t_width;
			this->height = t_height;
		}
		else {
			assert(width == t_width && height == t_height);
		}

		GLenum pixelFormat = t_pixelFormat == 0 ? pixelFormats[n] : t_pixelFormat;
		gl::TextureSubImage3D(handle, 0, 0, 0, i, width, height, 1, pixelFormat, gl::UNSIGNED_BYTE, data);
	}
}

void Texture3D::loadFromHDRFile(const std::string& file, GLenum t_internalFormat) {
	this->internalFormat = t_internalFormat;
	

	stbi_set_flip_vertically_on_load(true);
	Texture2D hdrTexture(file, internalFormat, TextureSettings(NoMipmaps, ClampToEdge, Linear));
	stbi_set_flip_vertically_on_load(false);

	allocate(internalFormat, { 2024, 2024 });

	Refactor::Framebuffer captureFBO;

	Shader equiToCubeShader("shaders/process/equiToCube.vert", "shaders/process/equiToCube.frag");

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.f), 1.0f, 0.1f, 1000.f);
	glm::mat4 captureViews[] = {
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	equiToCubeShader.bind();
	equiToCubeShader.setUniform("tex2D_sourceMap", 0);
	hdrTexture.bind(0);

	equiToCubeShader.setUniform("projection", captureProjection);

	gl::Viewport(0, 0, 2024, 2024);
	captureFBO.bind();

	auto cube = Mesh::generateCube(glm::vec3(1));

	for (uint8_t i = 0; i < 6; i++) {
		equiToCubeShader.setUniform("view", captureViews[i]);
		captureFBO.attach(gl::COLOR_ATTACHMENT0, gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, *this);

		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
		cube.draw();
	}
	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}



uint32_t Texture3D::impl_getMipmapLevels(bool useMipmaps, uint32_t width, uint32_t height) noexcept {
	if (useMipmaps == false) return 1;
	const uint32_t max = std::max(width, height);
	return 1 + int(std::floor(std::log2(max)));
}