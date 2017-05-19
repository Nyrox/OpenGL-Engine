#include "GLMInterface.h"
#include <angelscript/angelscript.h>
#include <glm/glm.hpp>
#include <new>

void vec2Constructor(void* memory) {
	new(memory) glm::vec2();
}

void vec2Destructor(void* memory) {

}

void vec2_registerObjectInterface(asIScriptEngine* engine) {
	engine->RegisterObjectType("Vector2", sizeof(glm::vec2), asOBJ_VALUE | asGetTypeTraits<glm::vec2>());
	engine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void Vector2()", asFUNCTION(vec2Constructor), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("Vector2", asBEHAVE_DESTRUCT, "void Vector2()", asFUNCTION(vec2Destructor), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectProperty("Vector2", "float x", asOFFSET(glm::vec2, x));
	engine->RegisterObjectProperty("Vector2", "float y", asOFFSET(glm::vec2, y));

	engine->RegisterObjectMethod("Vector2", "Vector2& opAssign(Vector2 &in)", asMETHODPR(glm::vec2, operator=, (const glm::vec2&), glm::vec2&), asCALL_THISCALL);
}



void vec3Constructor(void* memory) {
	new(memory) glm::vec3();
}

void vec3Destructor(void* memory) {

}

void vec3_registerObjectInterface(asIScriptEngine* engine) {
	engine->RegisterObjectType("Vector3", sizeof(glm::vec3), asOBJ_VALUE | asGetTypeTraits<glm::vec3>());
	engine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void Vector3()", asFUNCTION(vec3Constructor), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour("Vector3", asBEHAVE_DESTRUCT, "void Vector3()", asFUNCTION(vec3Destructor), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectProperty("Vector3", "float x", asOFFSET(glm::vec3, x));
	engine->RegisterObjectProperty("Vector3", "float y", asOFFSET(glm::vec3, y));
	engine->RegisterObjectProperty("Vector3", "float z", asOFFSET(glm::vec3, z));

	engine->RegisterObjectMethod("Vector3", "Vector3 opMul(float &in) const", asFUNCTIONPR(glm::operator*, (glm::vec3 const&, float const&), glm::vec3), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("Vector3", "Vector3& opAddAssign(Vector3 &in)", asMETHODPR(glm::vec3, operator+=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);
	engine->RegisterObjectMethod("Vector3", "Vector3& opSubAssign(Vector3 &in)", asMETHODPR(glm::vec3, operator-=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL);

}