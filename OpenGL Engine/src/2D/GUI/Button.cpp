#include "Button.h"
#include <Core\Shader.h>
#include <glm\gtx\transform.hpp>

Button::Button(glm::vec2 t_size) : box(t_size) {

}

void Button::setSize(glm::vec2 t_size) {
	box.setSize(t_size);
}

void Button::render(Shader& shader) {
	shader.setUniform("model", glm::translate(glm::vec3(transform.position)));
	box.render(shader);
}

void Button::handleEvent(const Event& event) {
	switch (event.type) {
	case Event::Click:
		if (box.getLocalBounds().contains(event.click.x, event.click.y)) {
			if (click_callback != nullptr) click_callback();
		}
		break;
	}


}