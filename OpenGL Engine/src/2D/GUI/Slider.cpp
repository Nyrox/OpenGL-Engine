#include "Slider.h"
#include <Core\Shader.h>
#include <algorithm>

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
	static bool isMouseDown;

	switch (event.type) {
	case Event::MouseDown:
		if (selector.getLocalBounds().contains(event.click.x, event.click.y)) {
			isMouseDown = true;
		}
		break;
	case Event::MouseUp:
		isMouseDown = false;
		break;
	case Event::MouseMove:
		if (isMouseDown) {
			float d = transform.position.x + size.x;
			float interp = (event.mouse.x - transform.position.x) / d;
			current = std::clamp(interp, 0.f, 1.f);

			sliderCallback(Math::lerp(valueRange.x, valueRange.y, current));
		}
		break;
	}
}