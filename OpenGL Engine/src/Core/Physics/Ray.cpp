#include "Ray.h"
#include <Core\Camera.h>
#include <glm\gtx\transform.hpp>

using namespace Physics;

Ray Physics::screenPositionToRay(const Camera& camera, glm::vec2 position) {
	// 3d normalized device coords
	glm::vec3 ray_device = { (2.0f * position.x) / camera.viewportWidth - 1.0f, 1.0f - (2.0f * position.y) / camera.viewportHeight, 1.0f };
	// Homogenous clip coords
	glm::vec4 ray_clip = glm::vec4(ray_device.x, ray_device.y, 1.0f, 1.0f);
	// View space
	glm::vec4 ray_view = glm::inverse(camera.projection) * ray_clip;
	ray_view = glm::vec4(ray_view.x, ray_view.y, -1.0, 0.0);
	// World space
	glm::vec3 ray_world = glm::vec3(glm::inverse(camera.getViewMatrix()) * ray_view);
	return Ray(camera.transform.position, glm::normalize(ray_world));
}