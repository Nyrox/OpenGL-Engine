#include "Slider.h"
#include <Core\Shader.h>

Slider::Slider(glm::vec2 t_size, glm::vec2 t_valueRange, std::function<void(double)> callback) 
	: size(t_size), valueRange(t_valueRange), sliderCallback(callback), box(size), selector(t_size.y / 2.5, 16) {

	box.fillColor = { 0.2, 0.2, 0.2 };
	selector.fillColor = { 0.9, 0.9, 0.9 };
}

void Slider::render(Shader& shader) {
	box.transform.position = { size.x, size.y, 0 };
	selector.transform.position = { transform.position + glm::vec3(size.x * current, size.y / 2, 0) };

	box.render(shader);
	selector.render(shader);
}

void Slider::handleEvent(const Event& event) {

}