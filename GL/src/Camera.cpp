#include "Camera.h"

Camera::Camera() {
	yaw = 0;
	pitch = 0;
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + forwards(), { 0, 1, 0});
}

glm::vec3 Camera::forwards() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	return normalize(front);
}

glm::vec3 Camera::right() {
	return glm::normalize(glm::cross(forwards(), { 0, 1, 0 }));
}