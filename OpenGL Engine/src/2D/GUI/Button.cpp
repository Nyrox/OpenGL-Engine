#include "Button.h"
#include <Core\Shader.h>
#include <glm\gtx\transform.hpp>

Button::Button(uint32_t t_width, uint32_t t_height) : width(t_width), height(t_height) {
	mesh = Mesh::generatePlane({ width, height });
}

void Button::render(Shader& shader) {
	shader.setUniform("model", glm::translate(glm::vec3(position, 0)));
	mesh.draw();
}

void Button::handleEvent(const Event& event) {
	switch (event.type) {
	case Event::Click:
		if (event.click.x > position.x && event.click.x < position.x + width &&
			event.click.y > position.y && event.click.y < position.y + height) {

			if (click_callback != nullptr) click_callback();
		}
		break;
	}


}