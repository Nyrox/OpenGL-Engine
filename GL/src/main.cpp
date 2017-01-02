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
#include <glm/gtc/type_ptr.hpp>

constexpr float CAMERA_NEAR_PLANE = 0.1f;
constexpr float CAMERA_FAR_PLANE = 100;

constexpr float SHADOWMAP_NEAR_PLANE = 0.1f;
constexpr float SHADOWMAP_FAR_PLANE = 100.f;


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

	
	glViewport(0, 0, 800, 600);

	Camera camera;
	camera.position.z = 5;
	camera.position.y = 2;

	glm::mat4 view = glm::translate(glm::mat4(), { 0, 0, -3 });
	glm::mat4 projection = glm::perspective(45.0f, 800.f / 600.f, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);

	Shader basicShader;
	basicShader.loadFromFile("shaders/basic.vert", "shaders/basic.frag");

	Shader flatShader;
	flatShader.loadFromFile("shaders/flat.vert", "shaders/flat.frag");

	Shader postProcessShader;
	postProcessShader.loadFromFile("shaders/post_process.vert", "shaders/post_process.frag");

	Shader shadowPassShader;
	shadowPassShader.loadFromFile("shaders/shadow_pass.vert", "shaders/shadow_pass.frag", "shaders/shadow_pass.geom");

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
	light2.diffuse = { 0.4, 0.4, 0.4 };
	light2.specular = { 0.4, 0.4, 0.4 };

	DirectionalLight dirLight;
	dirLight.direction = { -0.9f, -1.0f, -0.3f };
	dirLight.ambient = { 0.15, 0.15, 0.15 };
	dirLight.diffuse = { 0.6, 0.4, 0.4 };
	dirLight.specular = { 0.4, 0.4, 0.4 };

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, SHADOWMAP_NEAR_PLANE, SHADOWMAP_FAR_PLANE);

	Texture lampTexture;
	lampTexture.loadFromFile("assets/lamp_icon.png", GL_RGBA);
	Sprite lampIcon(1, 1, &lampTexture);
	Sprite lampIcon2(1, 1, &lampTexture);

	Mesh cube;
	cube.position.x -= 1.25;
	cube.position.z = 0;
	cube.position.y = 2;
	cube.material = Material(glm::vec3(0.8, 0.4, 0.31));
	cube.loadFromFile("assets/cube.ply");

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

	Texture brick_diffuse;
	brick_diffuse.loadFromFile("assets/meshes/brick_diffuse.png", GL_RGBA);

	Texture brick_specular;
	brick_specular.loadFromFile("assets/meshes/brick_specular", GL_RGBA);

	GLfloat deltaTime = 0;
	GLfloat lastFrame = 0;

	//Framebuffer postProcessBuffer({ 800, 600 }, RGB);
	Framebuffer shadowMapBuffer({ 1024, 1024 }, CUBE_DEPTH);
	Framebuffer shadowMapBuffer2({ 1024, 1024 }, CUBE_DEPTH);

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

	auto setCommonUniforms = [&](Shader& shader) {
		shader.setUniform("cameraPos", camera.position);
		
	};

	auto geometry_pass = [&](Shader& shader) {
		// Set common uniforms
		setCommonUniforms(shader);

		// render shit
		floor.draw(shader);
		cube.draw(shader);
		betterCube.draw(shader);
		cross.draw(shader);
	};

	auto physics_update = [&]() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*
			Input
		*/
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

		/*
			Logic
		*/
		light.position.z = sin(glfwGetTime()) * 2;
		light2.position.z += sin(glfwGetTime()) / 15;

		float adjacent = light.position.z - camera.position.z;
		float hypotenuse = std::sqrt(std::pow(adjacent, 2) + std::pow(light.position.x - camera.position.x, 2));
		lampIcon.yaw = glm::degrees(std::atan2(adjacent, hypotenuse)) + 45;

	};
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		/*
			Check for OpenGL errors
		*/
		for (GLenum err; (err = glGetError()) != GL_NO_ERROR;) {
			std::cout << err;
		}

		physics_update();

		
		shadowMapBuffer.bind();
		glViewport(0, 0, 1024, 1024);
		glClear(GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
		glEnable(GL_DEPTH_TEST);
		// Render
		
		//glm::mat4 dirLightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 lightSpaceMatrix = lightProjection * dirLightView;
	
		shadowPassShader.bind();
		//shadowPassShader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
		shadowPassShader.setUniform("far_plane", SHADOWMAP_FAR_PLANE);
		shadowPassShader.setUniform("lightPos", light.position);

		glm::mat4 shadowProj = glm::perspective(glm::radians(90.f), 1.f, SHADOWMAP_NEAR_PLANE, SHADOWMAP_FAR_PLANE);
		
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.position, light.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *	glm::lookAt(light.position, light.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *	glm::lookAt(light.position, light.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj *	glm::lookAt(light.position, light.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj *	glm::lookAt(light.position, light.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *	glm::lookAt(light.position, light.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		shadowPassShader.setUniformArray("shadowMatrices", shadowTransforms.data(), shadowTransforms.size());

		geometry_pass(shadowPassShader);

		
		shadowMapBuffer2.bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		shadowPassShader.setUniform("lightPos", light2.position);
		shadowTransforms.clear();
		shadowTransforms.push_back(shadowProj * glm::lookAt(light2.position, light2.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *	glm::lookAt(light2.position, light2.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *	glm::lookAt(light2.position, light2.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj *	glm::lookAt(light2.position, light2.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj *	glm::lookAt(light2.position, light2.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *	glm::lookAt(light2.position, light2.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		shadowPassShader.setUniformArray("shadowMatrices", shadowTransforms.data(), shadowTransforms.size());

		geometry_pass(shadowPassShader);


		// Lighting pass
		//postProcessBuffer.bind();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 800, 600);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
		glEnable(GL_DEPTH_TEST);

		basicShader.bind();
		basicShader.setUniform("cameraPos", camera.position);
		basicShader.setUniform("view", camera.getViewMatrix());
		basicShader.setUniform("projection", projection);
		//basicShader.setUniform("lightSpaceMatrix", lightSpaceMatrix);

		
		//basicShader.setUniform("pointLights", light, 0);
		//sicShader.setUniform("pointLights", light2, 1);
		//basicShader.setUniform("directionalLights", dirLight, 0);
		basicShader.setUniform("point_lights[0]", light);
		basicShader.setUniform("point_lights[1]", light2);

		basicShader.setUniform("point_light_count", 3);

		basicShader.setUniform("shadow_map_0", 2);
		basicShader.setUniform("shadow_map_1", 3);

		basicShader.setUniform("far_plane", (float)SHADOWMAP_FAR_PLANE);
		brick_diffuse.bind(0);
		brick_specular.bind(1);
		shadowMapBuffer.bindTexture(2);
		shadowMapBuffer2.bindTexture(3);

		geometry_pass(basicShader);

		
		lampIcon.position = light.position;
		lampIcon2.position = light2.position;


		
		flatShader.bind();
		flatShader.setUniform("view", camera.getViewMatrix());
		flatShader.setUniform("projection", projection);


		lampIcon.draw(flatShader);
		lampIcon2.draw(flatShader);
		
		/*
		glBindFramebuffer(GL_FRAMEBUFFER, 0);		
		glClearColor(1.0, 1.0, 1.0, 1.0); // Essentially useless
		glClear(GL_COLOR_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);

		
		postProcessShader.bind();
		postProcessShader.setUniform("near_plane", 0.1f);
		postProcessShader.setUniform("far_plane", 100.f);


		glBindVertexArray(vao);
		//shadowMapBuffer.bindTexture(0);
		postProcessBuffer.bindTexture(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		*/

		glfwSwapBuffers(window);
	}


	glfwTerminate();
}
