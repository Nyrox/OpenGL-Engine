#include "GUIContext.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GUIContext::GUIContext(uint32_t width, uint32_t height) : contextWidth(width), contextHeight(height) {
	shader.loadFromFile("shaders/stdgui.vert", "shaders/stdgui.frag");
}


void GUIContext::render() {
	glm::mat4 view_matrix = glm::ortho(0.f, (float)contextWidth, (float)contextHeight, 0.f, -1.f, 100.f);

	shader.bind();
	shader.setUniform("view", view_matrix);
	gl::Viewport(0, 0, 800, 600);

}
