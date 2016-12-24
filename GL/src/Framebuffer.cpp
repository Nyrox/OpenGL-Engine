#include "Framebuffer.h"
#include <iostream>

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &fbo);
}

Framebuffer::Framebuffer(glm::vec2 size, FramebufferTypes type, bool depthBuffer) : width(size.x), height(size.y) {
	glGenFramebuffers(1, &fbo);
	this->bind();

	if (type == DEPTH) depthBuffer = false;

	if (depthBuffer) {
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	}

	GLenum format = type == RGB ? GL_RGB : GL_DEPTH_COMPONENT;
	GLenum attach = type == RGB ? GL_COLOR_ATTACHMENT0 : GL_DEPTH_ATTACHMENT;

	glGenTextures(1, &tex);
	this->bindTexture();
	glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, tex, 0);

	if (type == DEPTH) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER: Initializing framebuffer failed." << std::endl;
	}
}

void Framebuffer::bind() {
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
}

void Framebuffer::bindTexture(uint32_t index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, tex);
}
