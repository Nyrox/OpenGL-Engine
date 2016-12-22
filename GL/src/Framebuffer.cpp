#include "Framebuffer.h"
#include <iostream>

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &fbo);
}

Framebuffer::Framebuffer(glm::vec2 size, FramebufferTypes type, bool depthBuffer) {
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

	if (type == DEPTH) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	GLenum format = type == RGB ? GL_RGB : GL_DEPTH_COMPONENT;
	GLenum attach = type == RGB ? GL_COLOR_ATTACHMENT0 : GL_DEPTH_ATTACHMENT;

	glGenTextures(1, &tex);
	this->bindTexture();
	glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, tex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER: Initializing framebuffer failed." << std::endl;
	}
}

void Framebuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::bindTexture(uint32_t index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, tex);
}
