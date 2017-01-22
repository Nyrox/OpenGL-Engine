#pragma once
#include <Shader.h>
#include <2D/Widget.h>

#include <vector>

class GUIContext {
public:
	GUIContext(uint32_t width, uint32_t height);

	void render();

	std::vector<Widget> widgets;

	Shader shader;
	uint32_t contextWidth, contextHeight;
};
