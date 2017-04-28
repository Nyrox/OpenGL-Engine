#pragma once
#include <glm\glm.hpp>
#include <gl_core_4_3.hpp>
#include <Core\Transform.h>
#include <Math\Math.h>

class Shader;

class Shape {
public:
	Shape::Shape();

	virtual glm::vec2 getPoint(uint32_t index) const = 0;
	virtual uint32_t getPointCount() const = 0;

	void render(Shader& shader) const;

	/*	Calculates the bounds of the shape in screen space, with only translation applied
		Be aware that this is not free. */
	FloatRect getLocalBounds() const;

	glm::vec4 fillColor;
	Transform transform;
protected:
	FloatRect localBounds;

	GLuint vao, vbo;

	void update();
};