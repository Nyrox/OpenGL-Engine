#pragma once
#include <gl_core_4_3.hpp>
#include <map>

class ImmediateDraw {
public:
	
	static void drawPlane(float width, float height);
private:
	std::map<std::pair<float, float>, GLuint> plane_cache;


	GLuint cachePlane(float width, float height);

	// Hide constructor because singletons etc.
	ImmediateDraw();
	ImmediateDraw(ImmediateDraw&) = delete;
	
	static ImmediateDraw& getInstance();
};