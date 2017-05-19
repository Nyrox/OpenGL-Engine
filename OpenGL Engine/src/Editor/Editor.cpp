#include "Editor.h"
#include <Core/AngelscriptAddons/ScriptSTDString/scriptstdstring.h>
#include <Core/Scripting/GLMInterface.h>

void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";
	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);

}

Editor::Editor(std::filesystem::path project) : window(1280, 720, "CloudEngine Editor"), input(window), camera(Transform({ 0, 0, 20 }), 1280, 720, glm::perspective(glm::radians(60.0f), 1280.f / 720.f, 0.1f, 500.f), -90, 0), renderer(engine, camera, 1280, 720) {
	engine.loadProject(project);

	scriptEngine = asCreateScriptEngine();
	scriptEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
	RegisterStdString(scriptEngine);
	
	vec2_registerObjectInterface(scriptEngine);
	vec3_registerObjectInterface(scriptEngine);

	Transform::__registerObjectInterface(scriptEngine);
	Camera::__registerObjectInterface(scriptEngine);
	Input::__registerObjectInterface(scriptEngine);

	scriptEngine->RegisterGlobalProperty("Camera camera", &camera);
	scriptEngine->RegisterGlobalProperty("Input input", &input);
	scriptEngine->RegisterGlobalProperty("float deltaTime", &deltaTime);


	editorScripts.emplace_back(scriptEngine, scriptBuilder, engine.getProjectBasePath() / "Editor/CameraController.as");
}

Editor::~Editor() {
	scriptEngine->ShutDownAndRelease();
}

void Editor::run() {
	while (window.isOpen()) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		input.update();

		for (auto& it : editorScripts) {
			it.context->Prepare(it.update);
			it.context->Execute();
		}

		renderer.render();
		window.display();
	}
}