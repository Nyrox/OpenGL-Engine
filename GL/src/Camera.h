#pragma once
#include <gl_core_4_3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera();

	glm::mat4 getViewMatrix();

	glm::vec3 position;

	float yaw;
	float pitch;

	glm::vec3 forwards();
	glm::vec3 right();
}; 