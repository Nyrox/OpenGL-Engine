#include <gl_core_4_3.hpp>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Camera.h>
#include <Shader.h>
#include <Texture.h>
#include <Cube.h>
#include <Plane.h>
#include <iostream>
#include <Mesh.h>
#include <2D/Sprite.h>
#include <Framebuffer.h>
#include <glm/gtc/type_ptr.hpp>

#include <Core/Renderer.h>
#include <2D/GUIContext.h>
#include <ImmediateDraw.h>
#include <Terrain.h>
#include <RessourceManager.h>
#include <Core\Image.h>
#include <Core\Skybox.h>

constexpr float CAMERA_NEAR_PLANE = 0.1f;
constexpr float CAMERA_FAR_PLANE = 10000;

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


void CALLBACK ErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << message << std::endl;
	
	if (severity == gl::DEBUG_SEVERITY_HIGH) {
		std::cin.get();
	}
}

#include <Editor\Editor.h>
#include <time.h>

int main() {

	srand(time(0));

	// GLFW
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	
	// Context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Hi", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create window." << std::endl;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	gl::sys::LoadFunctions();

	gl::Enable(gl::DEBUG_OUTPUT);
	gl::DebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(&ErrorCallback), NULL);

#ifdef _CUSTOM_EDITOR
	Editor editor(window);
	editor.run();

	glfwTerminate();
	return EXIT_SUCCESS;
#else

#endif

	gl::Viewport(0, 0, 1280, 720);

	Camera camera;
	camera.position.z = 5;
	camera.position.y = 10;

	Renderer renderer({ 1280, 720 });
	renderer.camera = &camera;

	glm::mat4 view = glm::translate(glm::mat4(), { 0, 0, -3 });
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), 1280.f / 720.f, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);

	renderer.projection = projection;

	PointLight light;
	light.position = { 2, 3, 2 };
	light.ambient = { 0.2, 0.2, 0.2 };
	light.diffuse = { 0.5, 0.5, 0.5 };
	light.specular = { 0.5, 0.5, 0.5 };

	PointLight light2;
	light2.position = { -3, 3, -1 };
	light2.ambient = { 0.2, 0.2, 0.2 };
	light2.diffuse = { 0.4, 0.4, 0.4 };
	light2.specular = { 0.4, 0.4, 0.4 };

	renderer.addPointLight(light);
	renderer.addPointLight(light2);

	DirectionalLight dirLight;
	dirLight.direction = { -0.9f, -1.0f, -0.3f };
	dirLight.ambient = { 0.15, 0.15, 0.15 };
	dirLight.diffuse = { 0.6, 0.4, 0.4 };
	dirLight.specular = { 0.4, 0.4, 0.4 };

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, SHADOWMAP_NEAR_PLANE, SHADOWMAP_FAR_PLANE);

	//Texture lampTexture;
	//lampTexture.loadFromFile("assets/lamp_icon.png", gl::RGBA);
	//Sprite lampIcon(1, 1, &lampTexture);
	//Sprite lampIcon2(1, 1, &lampTexture);

	Texture& texture = RessourceManager::loadTexture("container_diffuse", "assets/container2.png", gl::SRGB_ALPHA);
	Texture& specular = RessourceManager::loadTexture("container_specular", "assets/container2_specular.png", gl::SRGB_ALPHA);

	Mesh cube;
	cube.position.x -= 1.25;
	cube.position.z = 0;
	cube.position.y = 2;;
	cube.loadFromFile("assets/cube.ply");
	cube.material.diffuse = texture;
	
	Mesh betterCube;
	betterCube.position.x = -1.25;
	betterCube.position.z = 3;
	betterCube.position.y = 2;
	betterCube.loadFromFile("assets/cube.ply");
	betterCube.material.diffuse = texture;

	Mesh cross;
	cross.position.x = -1.25;
	cross.position.z = 6;
	cross.position.y = 1;
	cross.loadFromFile("assets/cross.ply");
	cross.material.diffuse = texture;

	RessourceManager::loadTexture("transparent", "assets/transparent.png", gl::SRGB_ALPHA);

	Mesh reflectiveCube;
	reflectiveCube.position.x = 3;
	reflectiveCube.position.z = 2;
	reflectiveCube.position.y = 2;

	reflectiveCube.loadFromFile("assets/cube.ply");
	reflectiveCube.material.diffuse = RessourceManager::getTexture("transparent");
	
	Mesh reflectiveSphere;
	reflectiveSphere.position.x = -4;
	reflectiveSphere.position.y = 2;

	reflectiveSphere.position.z = 3;
	reflectiveSphere.loadFromFile("assets/sphere.ply");
	reflectiveSphere.material.diffuse = RessourceManager::getTexture("transparent");

	renderer.meshes.push_back(cube);
	renderer.meshes.push_back(betterCube);
	renderer.meshes.push_back(cross);

	renderer.transparent.push_back(reflectiveSphere);
	renderer.transparent.push_back(reflectiveCube);

	
	GLfloat deltaTime = 0;
	GLfloat lastFrame = 0;

	GLuint vbo, vao;

	GLfloat vertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	gl::GenVertexArrays(1, &vao);
	gl::GenBuffers(1, &vbo);
	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(vertices), &vertices, gl::STATIC_DRAW);
	  
	gl::BindVertexArray(vao);
	// Positions
	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 2, gl::FLOAT, 0, 4 * sizeof(GLfloat), (GLvoid*)0);
	// UVS
	gl::EnableVertexAttribArray(1);
	gl::VertexAttribPointer(1, 2, gl::FLOAT, 0, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

	gl::BindVertexArray(0);

	auto setCommonUniforms = [&](Shader& shader) {
		shader.setUniform("cameraPos", camera.position);
		
	};

	bool rightMouseButtonIsDown = false;
	glm::vec2 cursorLastFrame;

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
		if (keys[GLFW_KEY_SPACE]) {
			camera.position.y += cameraSpeed;
		}
		if (keys[GLFW_KEY_LEFT_SHIFT]) {
			camera.position.y -= cameraSpeed;
		}


		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
			if (rightMouseButtonIsDown == false) { 
				rightMouseButtonIsDown = true; 
				double tmpx, tmpy;
				glfwGetCursorPos(window, &tmpx, &tmpy);
				cursorLastFrame = { tmpx, tmpy };
			}
			else {
				double tmpx, tmpy;
				glfwGetCursorPos(window, &tmpx, &tmpy);
				camera.yaw += (cursorLastFrame.x - tmpx) / -3;
				camera.pitch += (cursorLastFrame.y - tmpy) / 4;
				cursorLastFrame = { tmpx, tmpy };
			}
		}
		else {
			rightMouseButtonIsDown = false;
		}



		/*
			Logic
		*/
		light.position.z = sin(glfwGetTime()) * 2;
		light2.position.z += sin(glfwGetTime()) / 15;

		float adjacent = light.position.z - camera.position.z;
		float hypotenuse = std::sqrt(std::pow(adjacent, 2) + std::pow(light.position.x - camera.position.x, 2));
		//lampIcon.yaw = glm::degrees(std::atan2(adjacent, hypotenuse)) + 45;

	};
	
	GUIContext gui_context(1280, 720);

	
	Image heightmap;
	heightmap.loadFromFile("assets/heightmap.png");

	Terrain terrain(1000, 1000);
	terrain.generateMeshFromHeightmap(heightmap, 0.12);

	terrain.mesh.position = glm::vec3(-500, 10, -500);
	terrain.mesh.material.diffuse = RessourceManager::loadTexture("snow_diffuse", "assets/snow_diffuse.jpg", gl::SRGB);

	renderer.meshes.push_back(terrain.mesh);
	
	gl::DepthFunc(gl::LESS);
	gl::Enable(gl::DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		physics_update();

		texture.bind(0);
		specular.bind(1);

		renderer.render();
		gui_context.render();

		glfwSwapBuffers(window);
	}


	glfwTerminate();
}
