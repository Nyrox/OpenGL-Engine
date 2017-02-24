#pragma once
#include <2D\Shape.h>

class CircleShape : public Shape {
public:
	CircleShape(float radius, uint32_t vertexCount = 8);

	glm::vec2 getPoint(uint32_t index) const override;
	uint32_t getPointCount() const override;

	void setRadius(float radius);
	void setVertexCount(uint32_t vc);

	float getRadius() const	{ return radius; }
private:
	float radius;
	uint32_t vertexCount;

};