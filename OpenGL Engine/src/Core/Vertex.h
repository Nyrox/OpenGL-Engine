#pragma once
#include <glm\glm.hpp>

struct Vertex {
	Vertex() { };
	Vertex(glm::vec3 t_pos, glm::vec3 t_normal, glm::vec2 t_uv) : position(t_pos), normal(t_normal), uv(t_uv) { };
	
	
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};