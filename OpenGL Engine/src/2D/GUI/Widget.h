#pragma once
#include <2D\GUI\Event.h>

class Shader;


class Widget {
public:
	virtual void render(Shader& shader) { };
	virtual void handleEvent(const Event& event) { }

};