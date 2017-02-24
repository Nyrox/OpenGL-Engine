#pragma once
#include <2D/GUI/Widget.h>
#include <2D\RectangleShape.h>

#include <Core/Mesh.h>
#include <functional>


class Button : public Widget {
public:
	Button(glm::vec2 t_size);

	void setSize(glm::vec2 t_size);

	virtual void render(Shader& shader) override;
	virtual void handleEvent(const Event& event);

	std::function<void()> click_callback;

	Transform transform;
private:
	RectangleShape box;

};