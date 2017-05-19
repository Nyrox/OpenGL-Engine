#pragma once
#include <angelscript/angelscript.h>
#include <Core/AngelscriptAddons/ScriptBuilder/scriptbuilder.h>
#include <filesystem>

namespace std {
	namespace filesystem = std::experimental::filesystem;
}

struct EditorScript {
	EditorScript(asIScriptEngine* engine, CScriptBuilder& builder, std::filesystem::path path);
	~EditorScript();

	asIScriptModule* module;
	asIScriptFunction* update;
	asIScriptContext* context;
};