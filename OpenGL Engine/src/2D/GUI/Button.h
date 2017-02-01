#pragma once
#include <2D/GUI/Widget.h>
#include <2D/GUI/Sprite.h>


class Button : public Widget {
public:
	Button(uint32_t width, uint32_t height);

	void render(Shader& shader);
	
	Sprite sprite;

private:



};