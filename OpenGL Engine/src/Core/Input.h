#pragma once
#include <Core/Definitions.h>
#include <glm/glm.hpp>


class Window;
class asIScriptEngine;

class Input {
public:
	Input(Window& t_window);

	static void __registerObjectInterface(asIScriptEngine* engine);

	void update();
	
	bool isKeyDown(uint32 key);

	bool isMouseButtonDown(uint32 mouseButton);
	glm::vec2 getMousePosition();
	glm::vec2 getLastMousePosition();
	
	Window& window;
private:
	glm::vec2 mousePosition;
	glm::vec2 lastMousePosition;
};