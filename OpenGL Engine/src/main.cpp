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
#include <Editor\Gizmo.h>
#include <Core/Window.h>
#include <Core/Input.h>

#include <iostream>
#include <functional>

#include <Game\Village.h>
#include <Game\House.h>


#include <angelscript/angelscript.h>
#include <Core/AngelscriptAddons/ScriptSTDString/scriptstdstring.h>
#include <Core/AngelscriptAddons/ScriptBuilder/scriptbuilder.h>

#include <Core/Engine.h>
#include <Core/ECS/GameObject.h>
#include <Core/ECS/Components/MeshRenderer.h>

constexpr float CAMERA_NEAR_PLANE = 0.1f;
constexpr float CAMERA_FAR_PLANE = 500;

std::function<void(GLFWwindow*, int, int, int)> mouse_callback = nullptr;
std::function<void(GLFWwindow*, double, double)> cursorPositionCallback = nullptr;

void glfw_mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (mouse_callback != nullptr) mouse_callback(window, button, action, mods);
}

void glfw_cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
	if (cursorPositionCallback != nullptr) cursorPositionCallback(window, xPos, yPos);
}

#include <Editor\Editor.h>
#include <time.h>
#include <functional>
#include <fstream>

#include <Core\Physics\AABB.h>
#include <Core/JSON/json.h>
#include <filesystem>



#define _PROJECT_GAME 1


#include <Projects/GUI/guimain.h>

void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";
	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);

}

void print(const std::string& m) {
	std::cout << m << "\n";
}

void vec3Constructor(void* memory) {
	new(memory) glm::vec3();
}

void vec3Destructor(void* memory) {
	
}

void vec2Constructor(void* memory) {
	new(memory) glm::vec2();
}

void vec2Destructor(void* memory) {

}

glm::vec3& operator+=(glm::vec3& self, const glm::vec3& other) {
	self = other;
	return self;
}

int main() {
	srand(time(0));
#ifdef	_PROJECT_GUI 
	GUI::_main();
#elif _PROJECT_GAME

	Window window(1280, 720, "Praise kek", ContextSettings());
	Input input(window);

	glfwSetCursorPosCallback(window.handle, glfw_cursorPositionCallback);

#ifdef _CUSTOM_EDITOR
	Editor editor(window);
	editor.run();

	glfwTerminate();
	return EXIT_SUCCESS;
#endif
	Font arial("assets/fonts/arial.ttf", 48);
	Text fpsCounter(&arial);

	Camera camera(1280, 720, glm::perspective(glm::radians(60.0f), 1280.f / 720.f, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE), 0, 0);
	
	int r;
	asIScriptEngine* engine = asCreateScriptEngine();
	r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL); assert(r >= 0);

	RegisterStdString(engine);

	// glm::vec3
	engine->RegisterObjectType		("Vector3", sizeof(glm::vec3), asOBJ_VALUE | asGetTypeTraits<glm::vec3>());
	engine->RegisterObjectBehaviour	("Vector3", asBEHAVE_CONSTRUCT, "void Vector3()", asFUNCTION(vec3Constructor), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour	("Vector3", asBEHAVE_DESTRUCT, "void Vector3()", asFUNCTION(vec3Destructor), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectProperty	("Vector3", "float x", asOFFSET(glm::vec3, x));
	engine->RegisterObjectProperty	("Vector3", "float y", asOFFSET(glm::vec3, y));
	engine->RegisterObjectProperty	("Vector3", "float z", asOFFSET(glm::vec3, z));

	engine->RegisterObjectMethod	("Vector3", "Vector3 opMul(float &in) const", asFUNCTIONPR(glm::operator*, (glm::vec3 const&, float const&), glm::vec3), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod	("Vector3", "Vector3& opAddAssign(Vector3 &in)", asMETHODPR(glm::vec3, operator+=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);
	engine->RegisterObjectMethod	("Vector3", "Vector3& opSubAssign(Vector3 &in)", asMETHODPR(glm::vec3, operator-=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);

	// glm::vec2
	engine->RegisterObjectType		("Vector2", sizeof(glm::vec2), asOBJ_VALUE | asGetTypeTraits<glm::vec2>());
	engine->RegisterObjectBehaviour	("Vector2", asBEHAVE_CONSTRUCT, "void Vector2()", asFUNCTION(vec2Constructor), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour	("Vector2", asBEHAVE_DESTRUCT, "void Vector2()", asFUNCTION(vec2Destructor), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectProperty	("Vector2", "float x", asOFFSET(glm::vec2, x));
	engine->RegisterObjectProperty	("Vector2", "float y", asOFFSET(glm::vec2, y));

	engine->RegisterObjectMethod	("Vector2", "Vector2& opAssign(Vector2 &in)", asMETHODPR(glm::vec2, operator=, (const glm::vec2&), glm::vec2&), asCALL_THISCALL);

	

	Transform::__registerObjectInterface(engine);
	Camera::__registerObjectInterface(engine);
	Input::__registerObjectInterface(engine);

	engine->RegisterGlobalProperty("Camera camera", &camera);
	engine->RegisterGlobalProperty("Input input", &input);

	GLfloat deltaTime = 0;
	GLfloat lastFrame = 0;
	engine->RegisterGlobalProperty("float deltaTime", &deltaTime);


	CScriptBuilder builder;
	builder.StartNewModule(engine, "CameraController");
	builder.AddSectionFromFile("assets/scripts/camera.as");
	builder.BuildModule();
	
	asIScriptModule* mod = engine->GetModule("CameraController");
	asIScriptFunction* func = mod->GetFunctionByDecl("void update()");
	asIScriptContext* ctx = engine->CreateContext();

	TextureSettings highQualityTextureSettings(GenerateMipmaps, Repeat, Bilinear, 4.f);

	Engine ecs;
	Renderer renderer(ecs, camera, 1280, 720);

	ecs.loadProject("assets/projects/testProject");

	Image heightmap;
	heightmap.loadFromFile("assets/heightmap.png");

	Shader terrainShader("shaders/terrain.vert", "shaders/basic.frag");
	terrainShader.bind();
	terrainShader.setUniform("uvScale", 8.f);

	
	Texture2D blackrockAlbedo("assets/blackrock-albedo.png", gl::SRGB8, highQualityTextureSettings);
	Texture2D blackrockRoughness("assets/blackrock-roughness.png", gl::R8, highQualityTextureSettings);
	Texture2D blackrockMetal("assets/blackrock-metalness.png", gl::R8, highQualityTextureSettings);
	Texture2D blackrockNormal("assets/blackrock-normal.png", gl::RGB8, highQualityTextureSettings);

	Material terrainMaterial(Material::ShadingModel::PBR);
	terrainMaterial.uvScale = 8;
	terrainMaterial["albedo"] = &blackrockAlbedo;
	terrainMaterial["roughness"] = &blackrockRoughness;
	terrainMaterial["normal"] = &blackrockNormal;
	terrainMaterial["metal"] = &blackrockMetal;
		

	Terrain terrain(terrainMaterial, 400, 400);
	terrain.generateMeshFromHeightmap(heightmap, 0.000);

	terrain.model.transform.position = glm::vec3(-200, -1, -200);

	Scene scene;

	PointLight* light1 = scene.emplace<PointLight>(Transform(glm::vec3(2, 3, 2)), 1024, glm::vec3(1), 6);
	PointLight* light2 = scene.emplace<PointLight>(Transform(glm::vec3(-3, 3, 1)), 1024, glm::vec3(1), 6);

	renderer.addPointLight(light1);
	renderer.addPointLight(light2);

	Gizmo gizmo;

	bool rightMouseButtonIsDown = false;
	glm::vec2 cursorLastFrame;

	GUIContext gui_context(1280, 720);
	Button* exitButton = gui_context.createWidget<Button>(glm::vec2(50, 50));
	exitButton->transform.position.x = 1280 - 50;
	exitButton->click_callback = [&]() {
		glfwTerminate();
		exit(EXIT_SUCCESS);
	};

	//Slider* lightSlider = gui_context.createWidget<Slider>(glm::vec2(200, 25), glm::vec2(0, 1), [&](double val) {
	//});

	auto physics_update = [&]() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		gizmo.update();

		mouse_callback = [&](GLFWwindow* window, int button, int action, int mods) {
			if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
				Event event;
				event.mouse = { input.getMousePosition().x, input.getMousePosition().y };
				event.type = Event::MouseDown;
				event.mouse.camera = &camera;

				gui_context.handleEvent(event);
				
				if (!gizmo.handleEvent(event)) {
					return;
				}

				auto& results = scene.raycastAgainstSceneCollision(Physics::screenPositionToRay(camera, input.getMousePosition()));
				if (results.size()) {
					std::cout << "Selected new object: \n";
					gizmo.sceneNode = std::get<SceneNode*>(results.at(0));
				}

			}
			if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
				auto mousePosition = input.getMousePosition();
				Event event;
				event.click.x = mousePosition.x;
				event.click.y = mousePosition.y;
				event.click.camera = &camera;
				event.type = Event::Click;
				gui_context.handleEvent(event);
				gizmo.handleEvent(event);

				event.mouse = { mousePosition.x, mousePosition.y };
				event.type = Event::MouseUp;
				gui_context.handleEvent(event);
				gizmo.handleEvent(event);

				
				//Debug::drawLine(renderer.camera->position, renderer.camera->position + ray_world * 100.f, 15.f);

			}
		};
		cursorPositionCallback = [&](GLFWwindow* window, double xPos, double yPos) {
			Event event;
			event.mouse = { (float)xPos, (float)yPos };
			event.type = Event::MouseMove;
			event.mouse.camera = &camera;

			gui_context.handleEvent(event);
			gizmo.handleEvent(event);
		};

		glfwSetMouseButtonCallback(window.handle, glfw_mouse_callback);
	};




	gl::DepthFunc(gl::LESS);
	gl::Enable(gl::DEPTH_TEST);

	std::cout << "Starting game lopp." << std::endl;
	while (!glfwWindowShouldClose(window.handle)) {
		input.update();

		physics_update();
		scene.update();

		ctx->Prepare(func);
		r = ctx->Execute(); assert(r >= 0);

		fpsCounter.setString(std::to_string(std::round(1 / deltaTime)).substr(0, 2));

		renderer.render();
		gizmo.render(camera.getViewMatrix(), camera.projection);

		Debug::render(camera.getViewMatrix(), camera.projection);
		gui_context.render();
		scene.render(camera.getViewMatrix(), camera.projection);

		fpsCounter.render();

		window.display();
	}

	ctx->Release();
	engine->ShutDownAndRelease();
#endif
}
