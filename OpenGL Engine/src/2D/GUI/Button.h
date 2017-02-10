#pragma once
#include <2D/GUI/Widget.h>

#include <Core/Mesh.h>
#include <functional>


class Button : public Widget {
public:
	Button(uint32_t width, uint32_t height);

	virtual void render(Shader& shader) override;
	virtual void handleEvent(const Event& event);

	uint32_t width, height;
	glm::vec2 position;

	std::function<void()> click_callback;
private:
	Mesh mesh;

};