#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader.h>
#include <Cube.h>

#include <iostream>

int main() {
	// GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	
	// Context
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hi", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create window." << std::endl;
		std::cin.get();
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << glewGetErrorString(err) << std::endl;
		std::cin.get();
	}

	glm::mat4 view = glm::translate(glm::mat4(), { 0, 0, -3 });
	glm::mat4 projection = glm::perspective(45.0f, 800.f / 600.f, 0.1f, 100.f);

	Shader basicShader;
	basicShader.loadFromFile("shaders/basic.vert", "shaders/basic.frag");
	basicShader.bind();

	Cube cube;
	cube.position.x -= 0.25;
	cube.rotation.z = 25;
	cube.initRenderData();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		basicShader.setUniform("view", view);
		basicShader.setUniform("projection", projection);

		cube.draw(basicShader);

		glfwSwapBuffers(window);
	}


	glfwTerminate();
}