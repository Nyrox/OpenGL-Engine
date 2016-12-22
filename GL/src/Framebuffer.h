#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

enum FramebufferTypes {
	RGB, DEPTH
};

class Framebuffer {
public:
	Framebuffer(glm::vec2 size, FramebufferTypes type, bool depthBuffer = true);
	~Framebuffer();

	void bind();
	void bindTexture(uint32_t index = 0);

	GLuint fbo;
	GLuint tex;
private:

};

