#include "Window.h"


Window::Window(uint32 width, uint32 height, std::string title, const ContextSettings& settings) {
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings.debugContext);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.glVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (handle == nullptr) throw std::exception("glfwCreateWindow failed.");

	glfwMakeContextCurrent(handle);

	gl::sys::LoadFunctions();
	gl::Viewport(0, 0, width, height);

	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

}

Window::~Window() {
	glfwDestroyWindow(handle);
}

bool Window::isOpen() {
	return !glfwWindowShouldClose(handle);
}

void Window::close() {
	glfwSetWindowShouldClose(handle, true);
}

void Window::pollEvents() {
	glfwPollEvents();
}

void Window::display() {
	glfwSwapBuffers(handle);
}