#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <angelscript/angelscript.h>
	
#include <new>

struct Transform {
	explicit Transform(glm::vec3 t_position, glm::vec3 t_scale = glm::vec3(1))
		: position(t_position), scale(t_scale) {

	}
	explicit Transform() : scale(1) { }
	~Transform() = default;

	static void __registerObjectInterface(asIScriptEngine* engine) {
		engine->RegisterObjectType("Transform", sizeof(Transform), asOBJ_VALUE);
		engine->RegisterObjectBehaviour("Transform", asBEHAVE_CONSTRUCT, "void Transform()", asFUNCTION(__asConstructor), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectBehaviour("Transform", asBEHAVE_DESTRUCT, "void Transform()", asFUNCTION(__asDestructor), asCALL_CDECL_OBJLAST);
	
		engine->RegisterObjectProperty("Transform", "Vector3 position", asOFFSET(Transform, position));
	}

	glm::mat4 getModelMatrix() const {
		//return glm::translate(glm::scale(glm::mat4(), scale), position);
		return glm::scale(glm::translate(position), scale);
	}

	glm::vec3 position;
	glm::vec3 scale;

private:
	static void __asConstructor(void* memory) {
		new(memory) Transform();
	}

	static void __asDestructor(void* memory) {
		((Transform*)memory)->~Transform();
	}
};