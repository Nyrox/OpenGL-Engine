#include "Skybox.h"
#include <Core\Mesh.h>

#include <stb_image.h>
#include <glm\gtx\transform.hpp>

Skybox::Skybox() {
	shader.loadFromFile("shaders/skybox.vert", "shaders/skybox.frag");
}


void Skybox::loadFromFiles(const std::array<std::string, 6>& paths) {

	gl::GenTextures(1, &id);
	gl::BindTexture(gl::TEXTURE_CUBE_MAP, id);


	int32_t width, height, channels;
	unsigned char* data;
	for (uint32_t i = 0; i < paths.size(); i++) {
		data = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
		gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, gl::RGB, width, height, 0, gl::RGB, gl::UNSIGNED_BYTE, data);
	}

	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
	gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_EDGE);
}


void Skybox::render(glm::mat4 projection, glm::mat4 view) {

	gl::DepthMask(false);
	gl::Disable(gl::CULL_FACE);

	shader.bind();
	shader.setUniform("model", glm::translate(glm::mat4(), { -50, -50, -50 }));
	shader.setUniform("projection", projection);
	shader.setUniform("view", glm::mat4(glm::mat3(view)));

	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_CUBE_MAP, id);
	
	Mesh skybox = Mesh::generateCube(glm::vec3(100));
	skybox.draw();

	gl::DepthMask(true);

}