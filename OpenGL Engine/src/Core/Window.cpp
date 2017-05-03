#include "Window.h"
#include <iostream>

// callback for opengl errors
void __stdcall ErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	// ignore | --investigate later
	// Usage warning: Generic vertex attribute array 1 uses a pointer with a small value (0x0000000C). Is this intended to be used as an offset into a buffer object?
	if (id == 131076) return;


	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source) {
	case gl::DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case gl::DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case gl::DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case gl::DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case gl::DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case gl::DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type) {
	case gl::DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case gl::DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case gl::DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case gl::DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case gl::DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case gl::DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case gl::DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case gl::DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case gl::DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity) {
	case gl::DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case gl::DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case gl::DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case gl::DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

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

	if (settings.debugContext) {
		gl::Enable(gl::DEBUG_OUTPUT);
		gl::Enable(gl::DEBUG_OUTPUT_SYNCHRONOUS);

		gl::DebugMessageCallback(static_cast<GLDEBUGPROC>(ErrorCallback), nullptr);
		gl::DebugMessageControl(gl::DONT_CARE, gl::DONT_CARE, gl::DONT_CARE, 0, nullptr, 1);
	}
}

Window::~Window() {
	glfwDestroyWindow(handle);
	glfwTerminate();
}

bool Window::isOpen() {
	return !glfwWindowShouldClose(handle);
}

void Window::close() {
	glfwSetWindowShouldClose(handle, true);
}

void Window::display() {
	glfwSwapBuffers(handle);
}