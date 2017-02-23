#pragma once
#include <Core\Transform.h>
#include <gl_core_4_3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



class Camera {
public:
	Camera(Transform transform, float viewportWidth, float viewportHeight, glm::mat4 projection, float yaw = 0, float pitch = 0);	
	Camera(float viewportWidth, float viewportHeight, glm::mat4 projection, float yaw, float pitch);

	glm::mat4 getViewMatrix() const;	
	glm::vec3 forwards() const;
	glm::vec3 right() const;

	Transform transform;
	glm::mat4 projection;
	float yaw, pitch;
	float viewportWidth, viewportHeight;

private:
	Camera(const Camera& right) = delete;
	Camera(const Camera&& right) = delete;
	Camera operator=(const Camera& right) = delete;
}; 