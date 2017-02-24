#pragma once
#include <2D\GUI\Widget.h>
#include <2D\RectangleShape.h>
#include <2D\CircleShape.h>

#include <functional>

class Slider : public Widget {
public:
	Slider(glm::vec2 size, glm::vec2 valueRange, std::function<void(double)> callback = nullptr);

	virtual void render(Shader& shader) override;
	virtual void handleEvent(const Event& event);

	
	std::function<void(double)> sliderCallback;

	Transform transform;
	glm::vec2 valueRange;
	double current = 0.4;
private:
	RectangleShape box;
	CircleShape selector;

	glm::vec2 size;

};
