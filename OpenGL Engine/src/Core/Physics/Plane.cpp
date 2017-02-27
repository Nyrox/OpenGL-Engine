#include "Plane.h"
#include <Core\Physics\Ray.h>

using namespace Physics;

bool Plane::intersects(const Ray& ray, float& t, glm::vec3& q) {
	// This algorithm is actually a segment to plane test, so we first have to fake a line segment
	glm::vec3 ab = ray.origin - ray.origin + ray.direction * 1000.f;

	t = (glm::distance(glm::vec3(0), origin) - glm::dot(normal, ray.origin)) / glm::dot(normal, ab);

	// If t in [0..1] compute and return intersection point
	if (t >= 0.0f && t <= 1.0f) {
		q = ray.origin + t * ab;
		return 1;
	}
	// Else no intersection
	return 0;
}
