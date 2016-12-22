#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Camera.h>
#include <Shader.h>
#include <Texture.h>
#include <Cube.h>
#include <Plane.h>
#include <iostream>
#include <Mesh.h>
#include <2D/Sprite.h>
#include <Framebuffer.h>


bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		keys[key] = true;
		
		if (key == GLFW_KEY_ESCAPE) {
			exit(EXIT_SUCCESS);
		}
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

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
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << glewGetErrorString(err) << std::endl;
		std::cin.get();
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, 800, 600);

	Camera camera;
	camera.position.z = 5;
	camera.position.y = 2;

	glm::mat4 view = glm::translate(glm::mat4(), { 0, 0, -3 });
	glm::mat4 projection = glm::perspective(45.0f, 800.f / 600.f, 0.1f, 1000.f);

	Shader basicShader;
	basicShader.loadFromFile("shaders/basic.vert", "shaders/basic.frag");

	Shader flatShader;
	flatShader.loadFromFile("shaders/flat.vert", "shaders/flat.frag");

	Shader postProcessShader;
	postProcessShader.loadFromFile("shaders/post_process.vert", "shaders/post_process.frag");


	Plane floor;
	floor.scale = 2;
	floor.material = Material(glm::vec3(0.8, 0.4, 0.31));
	floor.initRenderData();
	
	PointLight light;
	light.position = { 2, 3, 2 };
	light.ambient = { 0.2, 0.2, 0.2 };
	light.diffuse = { 0.5, 0.5, 0.5 };
	light.specular = { 0.5, 0.5, 0.5 };

	PointLight light2;
	light2.position = { -6, 3, -1 };
	light2.ambient = { 0.2, 0.2, 0.2 };
	light2.diffuse = { 0.4, 0.4, 0.8 };
	light2.specular = { 0.4, 0.4, 0.8 };

	Texture lampTexture;
	lampTexture.loadFromFile("assets/lamp_icon.png", GL_RGBA);
	Sprite lampIcon(1, 1, &lampTexture);
	Sprite lampIcon2(1, 1, &lampTexture);

	Cube cube;
	cube.position.x -= 1.25;
	cube.position.z = 0;
	cube.position.y = 2;
	cube.material = Material(glm::vec3(0.8, 0.4, 0.31));
	cube.initRenderData();

	Mesh betterCube;
	betterCube.position.x = -1.25;
	betterCube.position.z = 3;
	betterCube.position.y = 2;

	betterCube.material = Material(glm::vec3(0.8, 0.4, 0.31));
	betterCube.loadFromFile("assets/cube.ply");

	Mesh cross;
	cross.position.x = -1.25;
	cross.position.z = 6;
	cross.position.y = 1;
	cross.material = Material(glm::vec3(0.8, 0.4, 0.31));
	cross.loadFromFile("assets/cross.ply");

	Texture texture;
	texture.loadFromFile("assets/container2.png", GL_RGBA);

	Texture specular;
	specular.loadFromFile("assets/container2_specular.png", GL_RGBA);

	GLfloat deltaTime = 0;
	GLfloat lastFrame = 0;

	Framebuffer postProcessBuffer({ 800, 600 }, RGB);
	Framebuffer shadowMap({ 800, 600 }, DEPTH);

	GLuint vbo, vao;

	GLfloat vertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	// Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, 0, 4 * sizeof(GLfloat), (GLvoid*)0);
	// UVS
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, 0, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

	glBindVertexArray(0);


	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		/*
			Check for OpenGL errors
		*/
		for (GLenum err; (err = glGetError()) != GL_NO_ERROR;) {
			std::cout << err;
		}

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// Physics
		GLfloat cameraSpeed = 5.f * deltaTime;
		if (keys[GLFW_KEY_W]) {
			camera.position += camera.forwards() * cameraSpeed;
		}
		if (keys[GLFW_KEY_S]) {
			camera.position -= camera.forwards() * cameraSpeed;
		}
		if (keys[GLFW_KEY_D]) {
			camera.position += camera.right() * cameraSpeed;
		}
		if (keys[GLFW_KEY_A]) {
			camera.position -= camera.right() * cameraSpeed;
		}
		if (keys[GLFW_KEY_E]) {
			camera.yaw += cameraSpeed * 25;
		}
		if (keys[GLFW_KEY_Q]) {
			camera.yaw -= cameraSpeed * 25;
		}

		// Globals
		basicShader.bind();
		basicShader.setUniform("cameraPos", camera.position);
		basicShader.setUniform("view", camera.getViewMatrix());
		basicShader.setUniform("projection", projection);
			
		
		postProcessBuffer.bind();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
		glEnable(GL_DEPTH_TEST);
		
		// Bind Textures
		texture.bind(0);
		specular.bind(1);

		
		// Render
		light.position.z = sin(glfwGetTime()) * 2;
		basicShader.setUniform("pointLights", light, 0);
		basicShader.setUniform("pointLights", light2, 1);

		floor.draw(basicShader);
		cube.draw(basicShader);
		betterCube.draw(basicShader);
		cross.draw(basicShader);

		lampIcon.position = light.position;
		lampIcon2.position = light2.position;

		/*
			TODO: Learn this shit
		*/
		float adjacent = light.position.z - camera.position.z;
		float hypotenuse = std::sqrt(std::pow(adjacent, 2) + std::pow(light.position.x - camera.position.x, 2));
		lampIcon.yaw = glm::degrees(std::atan2(adjacent, hypotenuse)) + 45;

		flatShader.bind();
		flatShader.setUniform("view", camera.getViewMatrix());
		flatShader.setUniform("projection", projection);


		lampIcon.draw(flatShader);
		lampIcon2.draw(flatShader);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);		
		glClearColor(1.0, 1.0, 1.0, 1.0); // Essentially useless
		glClear(GL_COLOR_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);

		
		postProcessShader.bind();

		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		postProcessBuffer.bindTexture(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);


		glfwSwapBuffers(window);
	}


	glfwTerminate();
}