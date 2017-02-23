#pragma once
#include <glm\glm.hpp>

class Camera;

namespace Physics {
	struct Ray {
		Ray(glm::vec3 t_origin, glm::vec3 t_direction) : origin(t_origin), direction(t_direction) {
			inverse_direction = 1.f / t_direction;
		};

		glm::vec3 origin, direction, inverse_direction;
	};


	/*
		Given a screen space position relative to the camera constructs a ray in that direction in world space	
	*/
	Ray screenPositionToRay(const Camera& camera, glm::vec2 position);
}