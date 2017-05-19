#pragma once
#include <Core/Camera.h>
#include <Core/Renderer.h>
#include <Core/Window.h>
#include <Core/Input.h>
#include <Core/Engine.h>
#include <Core/AngelscriptAddons/ScriptBuilder/scriptbuilder.h>
#include <Core/Scripting/EditorScript.h>

#include <GLFW/glfw3.h>

class Editor {
public:
	Editor(std::filesystem::path project);
	~Editor();

	void run();

private:
	Window window;
	Engine engine;
	Renderer renderer;


	Input input;
	Camera camera;


	enum eEditorState {
		E_SCENE_VIEW, E_GAME
	} state;

	// Editor scripts
	asIScriptEngine* scriptEngine;
	CScriptBuilder scriptBuilder;
	std::vector<EditorScript> editorScripts;

	GLfloat deltaTime = 0;
	GLfloat lastFrame = 0;
};