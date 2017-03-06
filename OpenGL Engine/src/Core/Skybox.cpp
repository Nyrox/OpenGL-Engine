#include "Skybox.h"
#include <Core\Mesh.h>

#include <stb_image.h>
#include <glm\gtx\transform.hpp>

#include <iostream>


Skybox::Skybox() : skyboxMesh(Mesh::generateCube(glm::vec3(100))) {
	shader.loadFromFile("shaders/skybox.vert", "shaders/skybox.frag");
}


void Skybox::loadFromFiles(const std::array<std::string, 6>& paths) {
	cubemap.loadFromFiles(paths, gl::RGB8);
}

void Skybox::loadFromHDRFile(const std::string& file) {
	cubemap.loadFromHDRFile(file, gl::RGB32F);
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

	skyboxMesh.draw();
	
	gl::DepthFunc(gl::LESS);
	gl::DepthMask(true);
}