#include "Shape.h"
#include <Core\Shader.h>

#include <vector>

Shape::Shape() {
	gl::CreateVertexArrays(1, &vao);
	gl::CreateBuffers(1, &vbo);

	gl::BindVertexArray(vao);
	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);

	gl::VertexAttribPointer(0, 2, gl::FLOAT, false, sizeof(float) * 2, 0);
	gl::EnableVertexAttribArray(0);

	gl::BindVertexArray(0);
}

void Shape::update() {	

	std::vector<glm::vec2> vertices;
	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;

	for (uint8_t i = 0; i < getPointCount(); i++) {
		glm::vec2 point = getPoint(i);
		vertices.push_back(point);

		// Update bounds
		if (point.x < minX) minX = point.x;
		else if (point.x > maxX) maxX = point.x;

		if (point.y < minY) minY = point.y;
		else if (point.y > maxY) maxY = point.y;
	}

	localBounds = FloatRect(minX, minY, maxX - minX, maxY - minY);

	gl::NamedBufferData(vbo, sizeof(glm::vec2) * vertices.size(), vertices.data(), gl::STATIC_DRAW);
}

void Shape::render(Shader& shader) const {
	shader.setUniform("fillColor", fillColor);
	shader.setUniform("model", transform.getModelMatrix());

	gl::BindVertexArray(vao);
	gl::DrawArrays(gl::TRIANGLE_FAN, 0, getPointCount());
	gl::BindVertexArray(0);
}

FloatRect Shape::getLocalBounds() const {
	FloatRect rect;
	rect.left = localBounds.left + transform.position.x;
	rect.top = localBounds.top + transform.position.y;
	rect.width = localBounds.width;
	rect.height = localBounds.height;
	return rect;
}