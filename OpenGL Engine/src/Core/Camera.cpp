#include "Camera.h"
#include <iostream>

Camera::Camera(float t_viewportWidth, float t_viewportHeight, glm::mat4 t_projection, float t_yaw, float t_pitch) :
	Camera(Transform(), t_viewportWidth, t_viewportHeight, t_projection, t_yaw, t_pitch) 
{

};

Camera::Camera(Transform t_transform, float t_viewportWidth, float t_viewportHeight, glm::mat4 t_projection, float t_yaw, float t_pitch) :
	transform(t_transform), viewportWidth(t_viewportWidth), viewportHeight(t_viewportHeight), projection(t_projection), yaw(t_yaw), pitch(t_pitch)
{

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