#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

struct Transform {
	Transform(glm::vec3 t_position = glm::vec3(), glm::vec3 t_scale = glm::vec3(1))
		: position(t_position), scale(t_scale) {

	}

	glm::mat4 getModelMatrix() const {
		//return glm::translate(glm::scale(glm::mat4(), scale), position);
		return glm::scale(glm::translate(position), scale);
	}

	glm::vec3 position;
	glm::vec3 scale;
};