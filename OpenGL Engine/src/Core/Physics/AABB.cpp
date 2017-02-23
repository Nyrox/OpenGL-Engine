#include "AABB.h"

using namespace Physics;

bool AABB::intersects(const Ray& ray, float* tmin, glm::vec3* q) const {
	*tmin = 0.f;  // set to -FLT_MAX to get first hit on line
	float tmax = std::numeric_limits<float>::max(); // max distance ray can travel
													// for all 3 slabs
	for (int i = 0; i < 3; i++)
	{
		if (abs(ray.direction[i]) < 0.0001) {
			// Ray is parallel to slab. No hit if origin not within slab
			if (ray.origin[i]  < this->min[i] || ray.origin[i] > this->max[i]) return false;
		}
		else
		{
			// Compute intersection t value of ray with near and far plane of slab
			float ood = 1.f / ray.direction[i];
			float t1 = (this->min[i] - ray.origin[i]) * ood;
			float t2 = (this->max[i] - ray.origin[i]) * ood;
			// make t1 be intersection with near plane, t2 with far plane
			if (t1 > t2) std::swap(t1, t2);
			// compute the intersection of slab intersection intervals
			*tmin = std::max(*tmin, t1);
			tmax = std::min(tmax, t2);
			// Exit with no collision as soon as slab intersection becomes empty
			if (*tmin > tmax) return false;
		}
	}
	// Ray intersects all 3 slabs. Return point (q) and intersection t value  (tmin)
	*q = ray.origin + ray.direction * *tmin;
	return true;
};

glm::vec3 AABB::operator[](std::size_t i) {
	if (i > 1) { throw std::out_of_range("Tried to access AABB bounds out of range"); }
	return i == 0 ? min : max;
};
