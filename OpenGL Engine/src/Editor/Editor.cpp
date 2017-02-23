#include "Editor.h"

Editor::Editor(GLFWwindow* _window) : window(_window){

}

void Editor::run() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();


	}
}