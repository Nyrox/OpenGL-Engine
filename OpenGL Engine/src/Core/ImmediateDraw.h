#pragma once
#include <gl_core_4_3.hpp>
#include <glm\glm.hpp>

#include <map>
#include <unordered_map>

class ImmediateDraw {
public:
	
	static void drawPlane(float width, float height);
	static void drawCube(glm::vec3 size);
private:
	std::map<std::pair<float, float>, GLuint> plane_cache;
	std::map<std::tuple<float, float, float>, GLuint> cube_cache;

	GLuint cachePlane(float width, float height);
	GLuint cacheCube(glm::vec3 size);


	// Hide constructor because singletons etc.
	ImmediateDraw();
	ImmediateDraw(ImmediateDraw&) = delete;
	
	static ImmediateDraw& getInstance();
};