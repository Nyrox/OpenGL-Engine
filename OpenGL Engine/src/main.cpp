// If you ever delete this comment you are literally hitler.
// lua LoadGen.lua -style=pointer_cpp -spec=gl -version=4.3 -profile=core core_4_3 -exts KHR_debug ARB_direct_state_access EXT_texture_filter_anisotropic
#include <gl_core_4_3.hpp>

// Comment this out if you want to build the game
//#define CL_IS_EDITOR
#ifdef CL_IS_EDITOR
#define CL_INCLUDE_DEBUG true
#else
// Comment this out if you want to build the game without debug functionality
#define CL_INCLUDE_DEBUG true
#endif

#include <GLFW/glfw3.h>

#include <Core\Renderer.h>
#include <Core\Debug.h>
#include <Core\Terrain.h>
#include <Core\Image.h>
#include <Core\Scene.h>
#include <2D\GUI\GUIContext.h>
#include <2D\Text.h>

#include <iostream>
#include <functional>

constexpr float CAMERA_NEAR_PLANE = 0.1f;
constexpr float CAMERA_FAR_PLANE = 10000;

std::function<void(GLFWwindow*, int, int, int)> mouse_callback = nullptr;
void glfw_mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (mouse_callback != nullptr) mouse_callback(window, button, action, mods);
}

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


void __stdcall ErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	// ignore | --investigate later
	// Usage warning: Generic vertex attribute array 1 uses a pointer with a small value (0x0000000C). Is this intended to be used as an offset into a buffer object?
	if (id == 131076) return;


	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)	{
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

#include <Editor\Editor.h>
#include <time.h>
#include <functional>

#include <Core\Physics\AABB.h>



class House : public SceneNode {
public:
	virtual Physics::AABB getSceneBoundingBox() const override {
		return collision;
	}

	Model model;
	Physics::AABB collision;
};


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
	gl::Enable(gl::DEBUG_OUTPUT_SYNCHRONOUS);
	gl::DebugMessageCallback(static_cast<GLDEBUGPROC>(ErrorCallback), nullptr);
	gl::DebugMessageControl(gl::DONT_CARE, gl::DONT_CARE, gl::DONT_CARE, 0, nullptr, 1);

#ifdef _CUSTOM_EDITOR
	Editor editor(window);
	editor.run();

	glfwTerminate();
	return EXIT_SUCCESS;
#else

#endif
	Font arial("assets/fonts/arial.ttf", 48);
	Text fpsCounter;
	fpsCounter.setFont(&arial);
	gl::Viewport(0, 0, 1280, 720);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), 1280.f / 720.f, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);
	Camera camera(1280, 720, projection, 0, 0);

	Renderer renderer(camera, 1280, 720);

	PointLight light;
	light.position = { 2, 3, 2 };
	light.ambient = { 0.2, 0.2, 0.2 };
	light.diffuse = { 0.5, 0.5, 0.5 };
	light.specular = { 0.5, 0.5, 0.5 };

	PointLight light2;
	light2.position = { -3, 3, -1 };
	light2.ambient = { 0.2, 0.2, 0.2 };
	light2.diffuse = { 0.7, 0.7, 0.7 };
	light2.specular = { 0.4, 0.4, 0.4 };

	renderer.addPointLight(light);
	renderer.addPointLight(light2);


	DirectionalLight dirLight;
	dirLight.direction = { -0.2f, -1.0f, -0.2f };
	dirLight.ambient = { 0.15, 0.15, 0.15 };
	dirLight.diffuse = { 1.3, 1.3, 1.3 };
	dirLight.specular = { 0.4, 0.4, 0.4 };

	renderer.addDirectionalLight(dirLight);

	Texture2D texture;
	texture.allocate(gl::SRGB8_ALPHA8, { 500, 500 });
	texture.loadFromFile("assets/container2.png", gl::RGBA);

	Texture2D transparent;
	transparent.allocate(gl::SRGB8_ALPHA8, { 1, 1 });
	transparent.loadFromFile("assets/transparent.png", gl::RGBA);

	Texture2D specular;
	specular.allocate(gl::SRGB8_ALPHA8, { 500, 500 });
	specular.loadFromFile("assets/container2_specular.png", gl::RGBA);
	
	Texture2D testSpecular;
	testSpecular.allocate(gl::R8, { 16, 16 });
	testSpecular.loadFromFile("assets/TestSpecular.png", gl::RED);

	Texture2D brickwallDiffuse;
	brickwallDiffuse.allocate(gl::SRGB8, { 1024, 1024 });
	brickwallDiffuse.loadFromFile("assets/brickwall.jpg", gl::RGB);

	Texture2D brickwallNormal;
	brickwallNormal.allocate(gl::RGB8, { 1024, 1024 });
	brickwallNormal.loadFromFile("assets/brickwall_normal.jpg", gl::RGB);

	std::shared_ptr<Mesh> cube_mesh = std::make_shared<Mesh>();
	cube_mesh->loadFromFile("assets/cube.ply");

	Model cube;
	cube.transform.position = glm::vec3(1.25, 0, 0);
	cube.mesh = cube_mesh;
	cube.material.diffuse = &brickwallDiffuse;
	cube.material.normal = &brickwallNormal;
	
	Model betterCube;
	betterCube.transform.position = glm::vec3(-1.25, 0, 3);
	betterCube.mesh = cube_mesh;
	betterCube.material.diffuse = &brickwallDiffuse;
	betterCube.material.normal = &brickwallNormal;

	Model cross;
	cross.transform.position = glm::vec3(-1.25, 0, 6);
	cross.mesh = std::make_shared<Mesh>();
	cross.mesh->loadFromFile("assets/cross.ply");
	cross.material.diffuse = &texture;

	Model reflectiveCube;
	reflectiveCube.transform.position = glm::vec3(3, 0, 2);
	reflectiveCube.mesh = cube_mesh;
	reflectiveCube.material.diffuse = &transparent;
	
	Model reflectiveSphere;
	reflectiveSphere.transform.position = glm::vec3(-4, 0, 3);
	reflectiveSphere.mesh = std::make_shared<Mesh>();
	reflectiveSphere.mesh->loadFromFile("assets/sphere.ply");
	reflectiveSphere.material.diffuse = &transparent;

	renderer.insert(&cube);
	renderer.insert(&betterCube);
	renderer.insert(&cross);

	renderer.insert(&reflectiveSphere);
	renderer.insert(&reflectiveCube);

	
	std::shared_ptr<Mesh> house_mesh = std::make_shared<Mesh>();
	house_mesh->loadFromFile("assets/house.ply");

	House house;
	house.model.transform.position.x = 20;
	house.model.mesh = house_mesh;
	house.model.material.diffuse = &texture;

	renderer.insert(&house.model);

	Image heightmap;
	heightmap.loadFromFile("assets/heightmap.png");

	Terrain terrain(400, 400);
	terrain.generateMeshFromHeightmap(heightmap, 0.008);

	Texture2D groundDiffuse(true, gl::REPEAT, gl::LINEAR_MIPMAP_LINEAR);
	groundDiffuse.allocate(gl::SRGB8, { 1024, 1024 });
	groundDiffuse.loadFromFile("assets/ground.png", gl::RGBA);

	terrain.model.transform.position = glm::vec3(-200, 0, -200);
	terrain.model.material.diffuse = &groundDiffuse;
	terrain.model.material.specular = &testSpecular;

	renderer.insert(&terrain.model);

	Scene scene;
	House* myHouse = scene.emplace<House>();
	myHouse->model.transform.position.x = 25;
	myHouse->model.mesh = house_mesh;
	myHouse->model.material.diffuse = &texture;

	renderer.insert(&myHouse->model);
	
	

	GLfloat deltaTime = 0;
	GLfloat lastFrame = 0;
	
	bool rightMouseButtonIsDown = false;
	glm::vec2 cursorLastFrame;

	GUIContext gui_context(1280, 720);
	Button* exitButton = gui_context.createWidget<Button>(200, 100);
	exitButton->position = { 0, 0 };
	exitButton->click_callback = [&]() {
		glfwTerminate();
		exit(EXIT_SUCCESS);
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
			camera.transform.position += camera.forwards() * cameraSpeed;
		}
		if (keys[GLFW_KEY_S]) {
			camera.transform.position -= camera.forwards() * cameraSpeed;
		}
		if (keys[GLFW_KEY_D]) {
			camera.transform.position += camera.right() * cameraSpeed;
		}
		if (keys[GLFW_KEY_A]) {
			camera.transform.position -= camera.right() * cameraSpeed;
		}
		if (keys[GLFW_KEY_E]) {
			camera.yaw += cameraSpeed * 25;
		}
		if (keys[GLFW_KEY_Q]) {
			camera.yaw -= cameraSpeed * 25;
		}
		if (keys[GLFW_KEY_SPACE]) {
			camera.transform.position.y += cameraSpeed;
		}
		if (keys[GLFW_KEY_LEFT_SHIFT]) {
			camera.transform.position.y -= cameraSpeed;
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

		mouse_callback = [&](GLFWwindow* window, int button, int action, int mods) {
			if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
				
				double mouse_x, mouse_y;
				glfwGetCursorPos(window, &mouse_x, &mouse_y);

				Event event;
				event.click.x = mouse_x;
				event.click.y = mouse_y;
				event.type = Event::Click;
				gui_context.handleEvent(event);

				Physics::Ray ray = Physics::screenPositionToRay(camera, glm::vec2(mouse_x, mouse_y));
				house.collision = Physics::AABB(house.model.transform.position - glm::vec3(1), house.model.transform.position + glm::vec3(1));
				Physics::AABB terrainCollision = Physics::AABB(terrain.model.transform.position, terrain.model.transform.position + glm::vec3(terrain.width, 0, terrain.height));
				float min;
				glm::vec3 q;
				std::cout << terrainCollision.intersects(ray, &min, &q) << std::endl;
				myHouse->model.transform.position = glm::vec3(q.x, house.model.transform.position.y, q.z);

				//Debug::drawLine(renderer.camera->position, renderer.camera->position + ray_world * 100.f, 15.f);
				
			}
		};
		glfwSetMouseButtonCallback(window, glfw_mouse_callback);
	};
	
	

	
	gl::DepthFunc(gl::LESS);
	gl::Enable(gl::DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		physics_update();
		
		fpsCounter.setString(std::to_string(std::round(1 / deltaTime)).substr(0, 2));

		specular.bind(1);

		renderer.render();
		Debug::render(camera.getViewMatrix(), camera.projection);
		gui_context.render();


		fpsCounter.render();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}
