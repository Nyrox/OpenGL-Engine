#pragma once
#include <gl_core_4_3.hpp>
#include <GLFW/glfw3.h>
#include <Core/ContextSettings.h>
#include <string>

class Window {
public:
	Window(uint32 width, uint32 height, std::string title, const ContextSettings& settings = ContextSettings());
	~Window();

	bool isOpen();
	void close();
	void display();

	GLFWwindow* handle;
};