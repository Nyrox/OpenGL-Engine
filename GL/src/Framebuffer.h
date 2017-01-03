#pragma once
#include <gl_core_4_3.hpp>
#include <glm/glm.hpp>

enum FramebufferTypes {
	RGB, DEPTH, CUBE_RGB, CUBE_DEPTH
};

class Framebuffer {
public:
	Framebuffer(glm::vec2 size, FramebufferTypes type, bool depthBuffer = true);
	Framebuffer(Framebuffer&& other);

	~Framebuffer();

	void bind();
	void bindTexture(uint32_t index = 0);

	GLuint fbo;
	GLuint tex;

	uint32_t width;
	uint32_t height;

	FramebufferTypes type;
private:

};

