#include "GUIContext.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

GUIContext::GUIContext(uint32_t width, uint32_t height) : contextWidth(width), contextHeight(height) {
	shader.loadFromFile("shaders/stdgui.vert", "shaders/stdgui.frag");
}


void GUIContext::render() {
	glm::mat4 projection = glm::ortho(0.f, (float)contextWidth, (float)contextHeight, 0.0f, -100.f, 100.f);

	shader.bind();
	shader.setUniform("projection", projection);

	gl::Disable(gl::CULL_FACE);
	for (auto& it : widgets) {
		it->render(shader);
	}

}


void GUIContext::handleEvent(Event event) {
	switch (event.type) {
	case Event::Click:
		//
		if (event.click.x > contextWidth || event.click.y > contextHeight) return;
		break;
	};

	for (auto& it : widgets) {
		it->handleEvent(event);
	};
}