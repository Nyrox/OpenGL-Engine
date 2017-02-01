#pragma once
#include <Core/Shader.h>
#include <vector>

class Debug {
public:

	// Draws a line from one point to another in world space
	// @[Params]
	// @start:		Start position of the line in world space
	// @end:		End position of the line in world space
	// @duration:	Time until the ray expires in seconds. (Duration 0 for no expiration)
	static void drawLine(glm::vec3 t_start, glm::vec3 t_end, float t_duration);

	// Render's the current debug context 
	// @[Params]
	// @view:		View matrix to use
	// @projection	Projection matrix to use
	static void render(glm::mat4 view, glm::mat4 projection);
private:
	// Data members
	Shader line_shader;
	std::vector<struct Line> lines;

	// Implementation
	void internal_renderLine(const Line& line, glm::mat4 view, glm::mat4 projection);

	// Hide constructors
	Debug();
	Debug(Debug&) = delete;

	static Debug& getInstance();
};