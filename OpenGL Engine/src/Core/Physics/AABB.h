#pragma once
#include "Ray.h"
#include <algorithm>
#include <stdexcept>

namespace Physics {

	class AABB {
	public:
		AABB(glm::vec3 t_min = glm::vec3(), glm::vec3 t_max = glm::vec3()) : min(t_min), max(t_max) { };

		bool intersects(const Ray& ray, float* tmin, glm::vec3* q) const;
		// Very ugly hack at incredibly low grace
		glm::vec3 operator[](std::size_t i);


		glm::vec3 min, max;
	};

}