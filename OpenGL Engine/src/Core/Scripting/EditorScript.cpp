#include "EditorScript.h"

EditorScript::EditorScript(asIScriptEngine* engine, CScriptBuilder& builder, std::filesystem::path path) {
	if (!std::filesystem::is_regular_file(path)) {
		throw std::runtime_error("Failed to load editor script: " + path.string());
	}

	builder.StartNewModule(engine, path.filename().string().c_str());
	builder.AddSectionFromFile(path.string().c_str());
	builder.BuildModule();

	module = engine->GetModule(path.filename().string().c_str());
	update = module->GetFunctionByDecl("void update()");
	context = engine->CreateContext();
}

EditorScript::~EditorScript() {
	context->Release();
}