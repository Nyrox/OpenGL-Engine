#include "RectangleShape.h"

RectangleShape::RectangleShape(glm::vec2 t_size, glm::vec2 t_position) : size(t_size), position(t_position) {
	update();
}

void RectangleShape::setSize(glm::vec2 t_size) {
	size = t_size;
	update();
}

glm::vec2 RectangleShape::getSize() {
	return size;
}

uint32_t RectangleShape::getPointCount() const  {
	return 4;
}

glm::vec2 RectangleShape::getPoint(uint32_t index) const {
	switch (index) {
		default:
		case 0: return glm::vec2(0, 0);
		case 1: return glm::vec2(size.x, 0);
		case 2: return glm::vec2(size.x, size.y);
		case 3: return glm::vec2(0, size.y);
	}
}