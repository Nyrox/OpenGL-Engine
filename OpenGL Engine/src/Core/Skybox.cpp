#include "Skybox.h"
#include <Core\Mesh.h>

#include <stb_image.h>
#include <glm\gtx\transform.hpp>

#include <iostream>
#include <Core/Framebuffer.h>

Skybox::Skybox() : skyboxMesh(Mesh::generateCube(glm::vec3(100))), cubemap(TextureSettings(NoMipmaps, ClampToEdge, Linear)) {
	shader.loadFromFile("shaders/skybox.vert", "shaders/skybox.frag");

}


void Skybox::loadFromFiles(const std::array<std::string, 6>& paths) {
	cubemap.loadFromFiles(paths, gl::RGB8);
}

void Skybox::loadFromHDRFile(const std::string& file) {
	cubemap.loadFromHDRFile(file, gl::RGB32F);

	convolutedCubemap.emplace(TextureSettings(NoMipmaps, ClampToEdge, Linear));

	Shader convolutionShader("shaders/process/convolute.vert", "shaders/process/convolute.frag");

	Refactor::Framebuffer captureFBO;
	convolutedCubemap->allocate(gl::RGB32F, { 32, 32 });

	convolutionShader.bind();
	convolutionShader.setUniform("tex2D_sourceMap", 0);
	cubemap.bind(0);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.f), 1.0f, 0.1f, 1000.f);
	glm::mat4 captureViews[] = {
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	convolutionShader.setUniform("projection", captureProjection);

	gl::Viewport(0, 0, 32, 32);
	captureFBO.bind();

	auto cube = Mesh::generateCube(glm::vec3(1));

	for (uint8_t i = 0; i < 6; i++) {
		convolutionShader.setUniform("view", captureViews[i]);
		captureFBO.attach(gl::COLOR_ATTACHMENT0, gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, *convolutedCubemap);

		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
		cube.draw();
	}

	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

}

void Skybox::render(glm::mat4 view, glm::mat4 projection) {

	gl::DepthMask(false);
	gl::Disable(gl::CULL_FACE);
	gl::DepthFunc(gl::LEQUAL);

	shader.bind();
	shader.setUniform("model", glm::translate(glm::vec3(-50)));
	shader.setUniform("projection", projection);
	shader.setUniform("view", glm::mat4(glm::mat3(view)));

	shader.setUniform("skybox", 0);
	cubemap.bind(0);
	//convolutedCubemap->bind(0);

	skyboxMesh.draw();
	
	gl::DepthFunc(gl::LESS);
	gl::DepthMask(true);
}