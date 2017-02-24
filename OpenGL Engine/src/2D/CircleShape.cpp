#include "CircleShape.h"
#include <Math/Math.h>

CircleShape::CircleShape(float t_radius, uint32_t t_vc) : radius(t_radius), vertexCount(t_vc) {
	update();
}

void CircleShape::setRadius(float t_radius) {
	radius = t_radius;
	update();
}

void CircleShape::setVertexCount(uint32_t t_vc) {
	vertexCount = t_vc;
	update();
}

uint32_t CircleShape::getPointCount() const {
	return vertexCount;
}

glm::vec2 CircleShape::getPoint(uint32_t index) const {
	float angle = index * 2 * Math::Pi / vertexCount - Math::Pi / 2;
	float x = std::cos(angle) * radius;
	float y = std::sin(angle) * radius;

	return glm::vec2(x, y);
}