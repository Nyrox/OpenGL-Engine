#include "Editor.h"

Editor::Editor(GLFWwindow* _window) : window(_window), editorRenderer(800, 600) {

}

void Editor::run() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();


	}
}