#include "Camera.h"

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + forwards(), { 0, 1, 0 });
}

glm::vec3 Camera::forwards() {
	return{ 0, 0, -1 };
}

glm::vec3 Camera::right() {
	return glm::normalize(glm::cross(forwards(), { 0, 1, 0 }));
}