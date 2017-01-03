#pragma once
#include <Shader.h>
#include <2D/Widget.h>

#include <vector>

class GUIContext {
public:
	GUIContext(float width, float height);

	void render();

	std::vector<Widget> widgets;


	float contextWidth, contextHeight;
};
