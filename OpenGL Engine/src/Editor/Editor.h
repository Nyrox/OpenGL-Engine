#pragma once
#include <Core/Camera.h>
#include <Core/Renderer.h>

#include <GLFW\glfw3.h>

class Editor {
public:
	Editor(GLFWwindow* window);

	void run();


private:
	GLFWwindow* window;


	// Preview

};