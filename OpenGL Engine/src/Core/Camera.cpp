#include "Camera.h"
#include <angelscript/angelscript.h>
#include <iostream>

Camera::Camera(float t_viewportWidth, float t_viewportHeight, glm::mat4 t_projection, float t_yaw, float t_pitch) :
	Camera(Transform(), t_viewportWidth, t_viewportHeight, t_projection, t_yaw, t_pitch) 
{

};

Camera::Camera(Transform t_transform, float t_viewportWidth, float t_viewportHeight, glm::mat4 t_projection, float t_yaw, float t_pitch) :
	transform(t_transform), viewportWidth(t_viewportWidth), viewportHeight(t_viewportHeight), projection(t_projection), yaw(t_yaw), pitch(t_pitch)
{

}

void Camera::__registerObjectInterface(asIScriptEngine* engine) {
	engine->RegisterObjectType("Camera", 0, asOBJ_REF | asOBJ_NOCOUNT);
	engine->RegisterObjectProperty("Camera", "float yaw", asOFFSET(Camera, yaw));
	engine->RegisterObjectProperty("Camera", "float pitch", asOFFSET(Camera, pitch));
	engine->RegisterObjectProperty("Camera", "float viewportWidth", asOFFSET(Camera, viewportWidth));
	engine->RegisterObjectProperty("Camera", "float viewportHeight", asOFFSET(Camera, viewportHeight));
	engine->RegisterObjectProperty("Camera", "Transform transform", asOFFSET(Camera, transform));

	engine->RegisterObjectMethod("Camera", "Vector3 forwards()", asMETHOD(Camera, forwards), asCALL_THISCALL);
	engine->RegisterObjectMethod("Camera", "Vector3 right()", asMETHOD(Camera, right), asCALL_THISCALL);

}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(transform.position, transform.position + forwards(), { 0, 1, 0});
}

glm::vec3 Camera::forwards() const {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	return normalize(front);
}

glm::vec3 Camera::right() const {
	return glm::normalize(glm::cross(forwards(), { 0, 1, 0 }));
}