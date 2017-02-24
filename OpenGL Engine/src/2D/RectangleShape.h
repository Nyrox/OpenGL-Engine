#pragma once
#include <2D\Shape.h>
#include <Core\Transform.h>

class RectangleShape : public Shape {
public:
	RectangleShape(glm::vec2 t_size);

	void setSize(glm::vec2 t_size);
	glm::vec2 getSize();

	virtual glm::vec2 getPoint(uint32_t index) const override;
	virtual uint32_t getPointCount() const override;

private:
	glm::vec2 size;


};