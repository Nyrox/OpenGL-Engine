#include "Input.h"
#include <Core/Window.h>
#include <GLFW/glfw3.h>
#include <angelscript/angelscript.h>


Input::Input(Window& t_window) : window(t_window) {

}

void Input::__registerObjectInterface(asIScriptEngine* engine) {
	engine->RegisterObjectType("Input", 0, asOBJ_REF | asOBJ_NOCOUNT);
	engine->RegisterObjectMethod("Input", "bool isKeyDown(uint32)", asMETHOD(Input, Input::isKeyDown), asCALL_THISCALL);
	engine->RegisterObjectMethod("Input", "bool isMouseButtonDown(uint32)", asMETHOD(Input, Input::isMouseButtonDown), asCALL_THISCALL);
	engine->RegisterObjectMethod("Input", "Vector2 getMousePosition()", asMETHOD(Input, Input::getMousePosition), asCALL_THISCALL);
	engine->RegisterObjectMethod("Input", "Vector2 getLastMousePosition()", asMETHOD(Input, Input::getLastMousePosition), asCALL_THISCALL);
	
	auto& registerEnums = [](std::initializer_list<std::pair<std::string, uint32>> values, asIScriptEngine* engine) {
		engine->RegisterEnum("Input_Keys");

		for (auto& it : values) {
			engine->RegisterEnumValue("Input_Keys", it.first.c_str(), it.second);
		}
	};

	registerEnums({
		{ "KEY_W", GLFW_KEY_W },
		{ "KEY_A", GLFW_KEY_A },
		{ "KEY_S", GLFW_KEY_S },
		{ "KEY_D", GLFW_KEY_D },
		{ "KEY_Q", GLFW_KEY_Q },
		{ "KEY_E", GLFW_KEY_E },
		{ "KEY_SPACE", GLFW_KEY_SPACE },
		{ "KEY_SHIFT", GLFW_KEY_LEFT_SHIFT },

		{ "MOUSE_BUTTON_1", GLFW_MOUSE_BUTTON_1 },
		{ "MOUSE_BUTTON_2", GLFW_MOUSE_BUTTON_2 }
	}, engine);

}

void Input::update() {
	glfwPollEvents();

	lastMousePosition = mousePosition;

	double tmpx, tmpy;
	glfwGetCursorPos(window.handle, &tmpx, &tmpy);
	this->mousePosition = { tmpx, tmpy };
}

bool Input::isKeyDown(uint32 key) {
	if (glfwGetKey(window.handle, key) == GLFW_PRESS) {
		return true;
	}

	return false;
}

bool Input::isMouseButtonDown(uint32 key) {
	if (glfwGetMouseButton(window.handle, key) == GLFW_PRESS) {
		return true;
	}

	return false;
}


glm::vec2 Input::getMousePosition() {
	return mousePosition;
}

glm::vec2 Input::getLastMousePosition() {
	return lastMousePosition;
}