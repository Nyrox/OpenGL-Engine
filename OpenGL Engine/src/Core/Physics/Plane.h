#pragma once
#include <Core\Physics\Ray.h>
#include <glm\glm.hpp>

/*
	Defines a plane of infinite size by an origin and a normal vector
*/

namespace Physics{
	
	
	class Plane {
	public:
		Plane(glm::vec3 t_origin = glm::vec3(), glm::vec3 t_normal = glm::vec3()) : origin(t_origin), normal(t_normal) { }
	
		bool intersects(const Ray& ray, float& t, glm::vec3& q);
	
		glm::vec3 origin;
		glm::vec3 normal;
	};

};